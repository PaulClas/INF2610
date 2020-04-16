# Sync Lab - Grader

import sys
import subprocess
import json
from yaml import load, Loader
import math

PRINT_ORDER = ['installer']

SCORES = {
    'installer': {
        'description': 'Scores for installer',
        'q1': {
            'description': 'Question 1: Per-package installer threads are correctly launched',
            'unitScale': 0.3,
            'scale': 3.0,
            'score': 0
        },
        'q2_sem_init': {
            'description': 'Question 2: Semaphore was correctly initialized',
            'scale': 1.0,
            'granted': False
        },
        'q2_packages_downloaded': {
            'description': 'Question 2: Packages have been downloaded correctly',
            'unitScale': 0.1,
            'scale': 1.0,
            'score': 0
        },
        'q2_packages_download_protected': {
            'description': 'Question 2: Package downloads are correctly protected by a semaphore',
            'unitScale': 0.1,
            'scale': 1.0,
            'score': 0
        },
        'q3_sem_init': {
            'description': 'Question 3: Semaphores were correctly initialized',
            'unitScale': 0.1,
            'scale': 1.0,
            'score': 0
        },
        'q3_packages_install_protected': {
            'description': 'Question 3: Package installs are correctly protected by semaphores',
            'unitScale': 0.2,
            'scale': 2.0,
            'score': 0
        },
        'q3_post_package_install': {
            'description': 'Question 3: Correctness of function postPackageInstall',
            'scale': 3.0,
            'score': 0,
            'timeout': False
        },
        'q4_test': {
            'description': 'Question 4: Failures are correctly handled',
            'unitScale': 0.2,
            'scale': 4.0,
            'score': 0
        }
    }
}

INSTALLER_LTRACEINFO = {
    'downSemAddr': '',
    'instSemAddr': ['' for i in range(10)],
    'pids': [-1 for i in range(10)],
    'downSemState': [0 for i in range(10)],
    'downState': [False for i in range(10)],
    'instSemState': [0 for i in range(10)],
    'instState': [False for i in range(10)]
}

INSTALLER_GRADEINFO = {
    'q2_packages_download_protected': [False for i in range(10)],
    'q3_packages_install_protected': [False for i in range(10)]
}

def handleReportLine(reportLine):
    key, token = reportLine.split(": ")

    if key == "installer" and token.startswith('q3_post_package'):
        tokenSpl = token.split()
        nbSuccess, nbTotal = int(tokenSpl[-2]), int(tokenSpl[-1])
        rawScore = SCORES['installer']['q3_post_package_install']['scale'] * nbSuccess / nbTotal
        SCORES['installer']['q3_post_package_install']['score'] = round(100 * rawScore) / 100
        return

    if 'countEvents' in SCORES[key][token]:
        SCORES[key][token]['countEvents'] = SCORES[key][token]['countEvents'] + 1
    elif 'granted' in SCORES[key][token]:
        SCORES[key][token]['granted'] = True
    elif 'unitScale' in SCORES[key][token]:
        SCORES[key][token]['score'] = SCORES[key][token]['score'] + SCORES[key][token]['unitScale']
    elif 'passed' in SCORES[key][token]:
        SCORES[key][token]['passed'] = True

def handleInstallerLtraceLine(ltraceLine):
    pid, info = ltraceLine.split(None, 1)
    try:
        if info.startswith("write@SYS(-1, ") or info.startswith("SYS_write(-1, "):
            writeInfo = info.split(", ")[1][1:-1]
            writeToken, writeArg = writeInfo.split(": ")
            if writeToken == 'threadedPackageInstaller':
                INSTALLER_LTRACEINFO['pids'][int(writeArg)] = int(pid)
            elif writeToken == 'downSemAddr':
                INSTALLER_LTRACEINFO['downSemAddr'] = writeArg
            elif writeToken.startswith('instSemAddr'):
                _, packageNum = writeToken.split('-')
                INSTALLER_LTRACEINFO['instSemAddr'][int(packageNum)] = writeArg
            elif writeToken == 'doPackageDownloadMarker':
                packageNum = int(writeArg)
                if int(pid) == INSTALLER_LTRACEINFO['pids'][packageNum]:
                    if INSTALLER_LTRACEINFO['downSemState'][packageNum] == -1:
                        INSTALLER_LTRACEINFO['downState'][packageNum] = True
            elif writeToken == 'doPackageInstallMarker':
                packageNum = int(writeArg)
                if int(pid) == INSTALLER_LTRACEINFO['pids'][packageNum]:
                    if INSTALLER_LTRACEINFO['instSemState'][packageNum] == -1:
                        INSTALLER_GRADEINFO['q3_packages_install_protected'][packageNum] = True
                        INSTALLER_LTRACEINFO['instState'][packageNum] = True
        elif info.startswith("sem_wait("):
            semAddr = info[9:].split(')')[0].split(', ')[0]
            if semAddr == INSTALLER_LTRACEINFO['downSemAddr']:
                packageNum = INSTALLER_LTRACEINFO['pids'].index(int(pid))
                INSTALLER_LTRACEINFO['downSemState'][packageNum] = INSTALLER_LTRACEINFO['downSemState'][packageNum] - 1
            else:
                packageNum = INSTALLER_LTRACEINFO['instSemAddr'].index(semAddr)
                if int(pid) == INSTALLER_LTRACEINFO['pids'][packageNum]:
                    INSTALLER_LTRACEINFO['instSemState'][packageNum] = INSTALLER_LTRACEINFO['instSemState'][packageNum] - 1
        elif info.startswith("sem_post("):
            semAddr = info[9:].split(')')[0].split(', ')[0]
            if semAddr == INSTALLER_LTRACEINFO['downSemAddr']:
                packageNum = INSTALLER_LTRACEINFO['pids'].index(int(pid))
                if INSTALLER_LTRACEINFO['downSemState'][packageNum] == -1:
                    if INSTALLER_LTRACEINFO['downState'][packageNum]:
                        INSTALLER_GRADEINFO['q2_packages_download_protected'][packageNum] = True
                INSTALLER_LTRACEINFO['downSemState'][packageNum] = INSTALLER_LTRACEINFO['downSemState'][packageNum] + 1
            else:
                packageNum = INSTALLER_LTRACEINFO['instSemAddr'].index(semAddr)
                if int(pid) == INSTALLER_LTRACEINFO['pids'][packageNum]:
                    INSTALLER_LTRACEINFO['instSemState'][packageNum] = INSTALLER_LTRACEINFO['instSemState'][packageNum] + 1

    except ValueError:
        return

def gradeInstaller():
    # Grade ltrace info
    for k, unitArray in INSTALLER_GRADEINFO.items():
        for b in unitArray:
            if b:
                SCORES['installer'][k]['score'] = SCORES['installer'][k]['score'] + SCORES['installer'][k]['unitScale']

def grade(gradeLogPath, installerLtraceLogPath, mode):
    handoutMode = True if mode == 'handout' else False

    with open(gradeLogPath, "r") as gradeLogFile:
        lines = gradeLogFile.readlines()

    for l in lines:
        handleReportLine(l.rstrip())

    with open(installerLtraceLogPath, "r") as installerLtraceLogFile:
        installerLtraceLogLines = installerLtraceLogFile.readlines()
    
    for l in installerLtraceLogLines:
        handleInstallerLtraceLine(l.rstrip())

    gradeInstaller()

    print("--------------------")
    print("Sync Lab: Autograder")
    print("--------------------")
    print("")

    totalScore = 0
    totalScale = 0
    scoresJSON = {
        'scores': {}
    }
    
    for problem in PRINT_ORDER:
        obj = SCORES[problem]
        print(obj['description'])
        print("---------------------------------------------------------------------------------------------")
        
        totalProblemScore = 0
        totalProblemScale = 0
        problemInvalidated = False
        for key, subscores in obj.items():
            if key  == 'description':
                continue
            
            if 'granted' in subscores:
                subscore = subscores['scale'] if subscores['granted'] else 0.0
            elif 'score' in subscores:
                # Round score
                subscores['score'] = round(subscores['score'] * 100) / 100
                subscore = subscores['score']
            elif 'passed' in subscores:
                if not subscores['passed']:
                    problemInvalidated = True
            else:
                subscore = 0
            if 'passed' in subscores:
                if subscores['passed']:
                    passedStr = 'PASSED'
                else:
                    passedStr = 'FAILED'
                row = ['-> ' + subscores['description'], str(passedStr)]
                print("{: <80} {: >11}".format(*row))
            else:
                row = ['-> ' + subscores['description'], str(subscore), "/" + str(subscores['scale'])]
                print("{: <80} {: >5} {: >5}".format(*row))

            if not 'passed' in subscores:
                totalProblemScore = totalProblemScore + subscore
                totalProblemScale = totalProblemScale + subscores['scale']
        
        # Round score
        totalProblemScore = round(totalProblemScore * 100) / 100

        if problemInvalidated:
            totalProblemScore = 0
        if problemInvalidated:
            row = ['TOTAL', 'FAILED']
            print("{: <80} {: >11}".format(*row))
        else:
            row = ['TOTAL', str(totalProblemScore), "/" + str(totalProblemScale)]
            print("{: <80} {: >5} {: >5}".format(*row))
        print("")

        totalScore = totalScore + totalProblemScore
        totalScale = totalScale + totalProblemScale
        scoresJSON['scores'][problem] = totalProblemScore
    
    print("---------------------------------------------------------------------------------------------")
    row = ['Total score', str(totalScore), "/" + str(totalScale)]
    print("{: <80} {: >5} {: >5}".format(*row))
    print("---------------------------------------------------------------------------------------------")
    if not handoutMode:
        print(json.dumps(scoresJSON))


if __name__ == '__main__':
    grade(sys.argv[1], sys.argv[2], sys.argv[3])
