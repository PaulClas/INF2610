# Clone Lab - Grader

import sys
import subprocess
import json
from yaml import load, Loader
from difflib import SequenceMatcher
import math

PRINT_ORDER = ['q1.1', 'q2.1', 'q2.2', 'q2.3', 'q2.4']

SCORES = {
    'q1.1': {
        'description': 'Scores for question 1.1',
        '1.1': {
            'description': 'Process 1 launched with the right syntax',
            'granted': False,
            'scale': 0.5
        },
        '1.2': {
            'description': 'Process 1 has the right PID for process 2',
            'granted': False,
            'scale': 0.5
        },
        '1.3': {
            'description': 'Process 1 received message back from process 2',
            'granted': False,
            'scale': 0.5
        },
        '2.1': {
            'description': 'Process 2 launched with the right syntax',
            'granted': False,
            'scale': 0.5
        },
        '2.2': {
            'description': 'Process 2 received message from process 1',
            'granted': False,
            'scale': 0.5
        }
    },
    'q2.1': {
        'description': 'Scores for question 2.1',
        'registerProc': {
            'description': 'Process tree and registerProc calls are correct',
            'unitScale': 0.5,
            'scale': 4.5,
            'score': 0
        },
    },
    'q2.2': {
        'description': 'Scores for question 2.2',
        'launchProc': {
            'description': 'exec calls and global behaviour are correct',
            'unitScale': 0.4,
            'scale': 4.0,
            'score': 0
        },
    },
    'q2.3': {
        'description': 'Scores for question 2.3',
        '1_stdout': {
            'description': 'fprintf messages are correct',
            'unitScale': 0.3,
            'scale': 3.0,
            'score': 0
        },
        '2_constraint': {
            'description': 'fprintf constraint is respected',
            'passed': True
        },
    },
    'q2.4': {
        'description': 'Scores for question 2.4',
        'mystery': {
            'description': 'Mystery is solved...',
            'granted': False,
            'scale': 2.0,
        }
    }
}

PARAMS = {}
PART2_PROCESSTREE = {}
REPORT_STATUS = {
    'ignore': False
}

def handleReportLine(reportLine):
    # Little trick here to ignore second execution of "./clonelab 2" in the grading script
    if REPORT_STATUS['ignore']:
        return
    if reportLine == 'ENDREPORT':
        REPORT_STATUS['ignore'] = True
        return
    key, token = reportLine.split(": ")
    if key == 'part2':
        if token.find('registerProc') != -1:
            info = token.split('registerProc ')[1].split(', ')
            
            if info[0] == '1' and int(info[1]) <= len(PARAMS['PART2_PROCESS_TREE']['children']):
                paramsTreeNode = PARAMS['PART2_PROCESS_TREE']['children'][int(info[1]) - 1]
            elif info[0] == '2':
                parentProcNum = 1
                childProcNum = int(info[1])
                while parentProcNum <= len(PARAMS['PART2_PROCESS_TREE']['children']) and childProcNum > len(PARAMS['PART2_PROCESS_TREE']['children'][parentProcNum - 1]['children']):
                    childProcNum = childProcNum - len(PARAMS['PART2_PROCESS_TREE']['children'][parentProcNum - 1]['children'])
                    parentProcNum = parentProcNum + 1
                if not parentProcNum <= len(PARAMS['PART2_PROCESS_TREE']['children']):
                    return
                if childProcNum <= len(PARAMS['PART2_PROCESS_TREE']['children'][parentProcNum - 1]['children']):
                    paramsTreeNode = PARAMS['PART2_PROCESS_TREE']['children'][parentProcNum - 1]['children'][childProcNum - 1]
            else:
                return
            
            paramsTreeNode['declaredPid'] = int(info[2])
            paramsTreeNode['declaredPpid'] = int(info[3])

            return
        if token.find('registerExec') != -1:
            info = token.split('registerExec ')[1].split(', ')
            if info[0] == 'level0':
                originPid = int(info[1])
                if originPid == PART2_PROCESSTREE['pid']:
                    SCORES['q2.2']['launchProc']['score'] = SCORES['q2.2']['launchProc']['score'] + SCORES['q2.2']['launchProc']['unitScale']
            elif info[0].find('level1.') != -1:
                originPid = int(info[1])
                childProcNum = int(info[0].split('level1.')[1])
                if not childProcNum <= len(PART2_PROCESSTREE['children']):
                    return
                if originPid == PART2_PROCESSTREE['children'][childProcNum - 1]['pid']:
                    SCORES['q2.2']['launchProc']['score'] = SCORES['q2.2']['launchProc']['score'] + SCORES['q2.2']['launchProc']['unitScale']
            elif info[0].find('level2.') != -1:
                originPid = int(info[1])
                childProcNum = int(info[0].split('level2.')[1])
                parentProcNum = 1
                while parentProcNum <= len(PARAMS['PART2_PROCESS_TREE']['children']) and childProcNum > len(PARAMS['PART2_PROCESS_TREE']['children'][parentProcNum - 1]['children']):
                    childProcNum = childProcNum - len(PARAMS['PART2_PROCESS_TREE']['children'][parentProcNum - 1]['children'])
                    parentProcNum = parentProcNum + 1
                if not parentProcNum <= len(PARAMS['PART2_PROCESS_TREE']['children']) or not childProcNum <= len(PARAMS['PART2_PROCESS_TREE']['children'][parentProcNum - 1]['children']):
                    return
                if not parentProcNum <= len(PART2_PROCESSTREE['children']) or not childProcNum <= len(PART2_PROCESSTREE['children'][parentProcNum - 1]['children']):
                    return
                if originPid == PART2_PROCESSTREE['children'][parentProcNum - 1]['children'][childProcNum - 1]['pid']:
                    SCORES['q2.2']['launchProc']['score'] = SCORES['q2.2']['launchProc']['score'] + SCORES['q2.2']['launchProc']['unitScale']
            
            return

    if 'countEvents' in SCORES[key][token]:
        SCORES[key][token]['countEvents'] = SCORES[key][token]['countEvents'] + 1
    elif 'granted' in SCORES[key][token]:
        SCORES[key][token]['granted'] = True
    elif 'unitScale' in SCORES[key][token]:
        SCORES[key][token]['score'] = SCORES[key][token]['score'] + SCORES[key][token]['unitScale']

def handlePart2LtraceOutput(lines):
    cnt = 0
    for l in lines:
        spl = l.split(' ', 1)
        if spl[1].startswith('fprintf') or spl[1].startswith('fputs'):
            if spl[1].find('Root process has pid') != -1:
                cnt = cnt + 1
        if cnt > 1:
            break
    if cnt != 1:
        SCORES['q2.3']['2_constraint']['passed'] = False


def handlePart2StraceOutput(lines):
    pidToTreeNode = {}
    rootPid = 0

    for s in lines:
        if rootPid == 0:
            if s.find('exec') != -1:
                if s.find('clonelab') != -1:
                    rootPid = int(s.split()[0])
        if not PART2_PROCESSTREE:
            spl = s.split()
            if int(spl[0]) == rootPid:
                if s.find('clone') != -1:
                    if spl[-2] == '=' and int(spl[-1]) > 0:
                        PART2_PROCESSTREE['pid'] = int(spl[-1])
                        pidToTreeNode[int(spl[-1])] = PART2_PROCESSTREE
        else:
            if s.find('clone(') != -1 or s.find('clone resumed') != -1:
                spl = s.split()
                if spl[-2] == '=' and int(spl[-1]) > 0:
                    if not int(spl[-1]) in pidToTreeNode:
                        if not int(spl[0]) in pidToTreeNode:
                            pidToTreeNode[int(spl[0])] = {
                                'pid': int(spl[0])
                            }
                        parentProcTree = pidToTreeNode[int(spl[0])]
                        childProcTree = {
                            'pid': int(spl[-1])
                        }
                        if 'children' in parentProcTree:
                            parentProcTree['children'].append(childProcTree)
                        else:
                            parentProcTree['children'] = [childProcTree]
                        pidToTreeNode[int(spl[-1])] = childProcTree
                    else:
                        if not int(spl[0]) in pidToTreeNode:
                            pidToTreeNode[int(spl[0])] = {
                                'pid': int(spl[0])
                            }
                        parentProcTree = pidToTreeNode[int(spl[0])]
                        childProcTree = pidToTreeNode[int(spl[-1])]
                        if 'children' in parentProcTree:
                            parentProcTree['children'].append(childProcTree)
                        else:
                            parentProcTree['children'] = [childProcTree]
            if s.find('write(') != -1 and s.find('write(1, "') == -1:
                spl = s.split()
                pid = int(spl[0])
                if not pid in pidToTreeNode:
                    pidToTreeNode[pid] = {
                        'pid': pid
                    }
                treeNode = pidToTreeNode[pid]
                if 'fileOutStraceLines' in treeNode:
                    treeNode['fileOutStraceLines'].append(s)
                else:
                    treeNode['fileOutStraceLines'] = [s]



def gradePart2ProcessTree():
    paramsTreeNode = PARAMS['PART2_PROCESS_TREE']
    realTreeNode = PART2_PROCESSTREE
    currentParentPid = realTreeNode['pid']

    # Check level0 stdout
    expectedPid = realTreeNode['pid']
    expectedSentence = 'Root process has pid ' + str(expectedPid) + ' (message from process level0)\\n'
    if 'fileOutStraceLines' in realTreeNode:
        for s in realTreeNode['fileOutStraceLines']:
            if s.find('"' + expectedSentence + '"') != -1:
                SCORES['q2.3']['1_stdout']['score'] = SCORES['q2.3']['1_stdout']['score'] + SCORES['q2.3']['1_stdout']['unitScale']
                break

    for i in range(len(paramsTreeNode['children'])):
        if not 'children' in realTreeNode:
            continue
        if i < len(realTreeNode['children']):
            paramsNode = paramsTreeNode['children'][i]
            realNode = realTreeNode['children'][i]
            if 'declaredPid' in paramsNode and realNode['pid'] == paramsNode['declaredPid']:
                if 'declaredPpid' in paramsNode and paramsNode['declaredPpid'] == currentParentPid:
                    SCORES['q2.1']['registerProc']['score'] = SCORES['q2.1']['registerProc']['score'] + SCORES['q2.1']['registerProc']['unitScale']
                    # Correct score if overflow
                    if SCORES['q2.1']['registerProc']['score'] > SCORES['q2.1']['registerProc']['scale']:
                        SCORES['q2.1']['registerProc']['score'] = SCORES['q2.1']['registerProc']['scale']

                    # Check stdout
                    if paramsNode['print']:
                        expectedName = paramsNode['name']
                        expectedSentence = 'Root process has pid ' + str(expectedPid) + ' (message from process ' + expectedName + ')\\n'
                        if 'fileOutStraceLines' in realNode:
                            for s in realNode['fileOutStraceLines']:
                                if s.find('"' + expectedSentence + '"') != -1:
                                    SCORES['q2.3']['1_stdout']['score'] = SCORES['q2.3']['1_stdout']['score'] + SCORES['q2.3']['1_stdout']['unitScale']
                                    break
                    else:
                        if not 'fileOutStraceLines' in realNode:
                            SCORES['q2.3']['1_stdout']['score'] = SCORES['q2.3']['1_stdout']['score'] + SCORES['q2.3']['1_stdout']['unitScale']
                        else:
                            # Check if process erroneously outputed the sentence
                            granted = True
                            for s in realNode['fileOutStraceLines']:
                                if s.find('Root process has pid') != -1:
                                    granted = False
                            if granted:
                                SCORES['q2.3']['1_stdout']['score'] = SCORES['q2.3']['1_stdout']['score'] + SCORES['q2.3']['1_stdout']['unitScale']
            currentParentDeclaredPid = realNode['pid']
            for j in range(len(paramsNode['children'])):
                if not 'children' in realNode:
                    continue
                if j < len(realNode['children']):
                    paramsSubNode = paramsNode['children'][j]
                    realSubNode = realNode['children'][j]
                    if 'declaredPid' in paramsSubNode and realSubNode['pid'] == paramsSubNode['declaredPid']:
                        if 'declaredPpid' in paramsSubNode and paramsSubNode['declaredPpid'] == currentParentDeclaredPid:
                            SCORES['q2.1']['registerProc']['score'] = SCORES['q2.1']['registerProc']['score'] + SCORES['q2.1']['registerProc']['unitScale']

                            # Check stdout
                            if paramsSubNode['print']:
                                expectedName = paramsSubNode['name']
                                expectedSentence = 'Root process has pid ' + str(expectedPid) + ' (message from process ' + expectedName + ')\\n'
                                if 'fileOutStraceLines' in realSubNode:
                                    for s in realSubNode['fileOutStraceLines']:
                                        if s.find('"' + expectedSentence + '"') != -1:
                                            SCORES['q2.3']['1_stdout']['score'] = SCORES['q2.3']['1_stdout']['score'] + SCORES['q2.3']['1_stdout']['unitScale']
                                            break
                            else:
                                if not 'fileOutStraceLines' in realSubNode:
                                    SCORES['q2.3']['1_stdout']['score'] = SCORES['q2.3']['1_stdout']['score'] + SCORES['q2.3']['1_stdout']['unitScale']
                                else:
                                    # Check if process erroneously outputed the sentence
                                    granted = True
                                    for s in realSubNode['fileOutStraceLines']:
                                        if s.find('Root process has pid') != -1:
                                            granted = False
                                    if granted:
                                        SCORES['q2.3']['1_stdout']['score'] = SCORES['q2.3']['1_stdout']['score'] + SCORES['q2.3']['1_stdout']['unitScale']



def grade(gradeLogPath, paramsFilePath, part2StraceLogPath, part2LtraceLogPath, mode):
    handoutMode = True if mode == 'handout' else False

    # Handling part 1
    with open(paramsFilePath, 'r', encoding='utf-8') as paramsFile:
        paramsAsStr = paramsFile.read()
        for key, obj in load(paramsAsStr, Loader=Loader).items():
            PARAMS[key] = obj
    with open(part2StraceLogPath, "r") as part2StraceLogFile:
        lines = part2StraceLogFile.readlines()
        handlePart2StraceOutput(lines)
    with open(part2LtraceLogPath, "r") as part2LtraceLogFile:
        lines = part2LtraceLogFile.readlines()
        handlePart2LtraceOutput(lines)

    with open(gradeLogPath, "r") as gradeLogFile:
        lines = gradeLogFile.readlines()

    for l in lines:
        handleReportLine(l.rstrip())

    # Grading part 1 process tree
    gradePart2ProcessTree()

    print("---------------------")
    print("Clone Lab: Autograder")
    print("---------------------")
    print("")

    totalScore = 0
    totalScale = 0
    bonusScore = 0
    bonusScale = 0
    scoresJSON = {
        'scores': {}
    }
    
    for problem in PRINT_ORDER:
        obj = SCORES[problem]
        print(obj['description'])
        print("------------------------------------------------------------------------------")
        
        totalProblemScore = 0
        totalProblemScale = 0
        bonusProblemScore = 0
        bonusProblemScale = 0
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
                print("{: <65} {: >11}".format(*row))
            else:
                if 'isBonus' in subscores and subscores['isBonus']:
                    row = ['-> ' + subscores['description'], "+" + str(subscore), "/" + str(subscores['scale'])]
                    print("{: <65} {: >5} {: >5}".format(*row))
                else:
                    row = ['-> ' + subscores['description'], str(subscore), "/" + str(subscores['scale'])]
                    print("{: <65} {: >5} {: >5}".format(*row))

            if not 'passed' in subscores:
                if 'isBonus' in subscores and subscores['isBonus']:
                    bonusProblemScore = bonusProblemScore + subscore
                    bonusProblemScale = bonusProblemScale + subscores['scale']
                else:
                    totalProblemScore = totalProblemScore + subscore
                    totalProblemScale = totalProblemScale + subscores['scale']
        
        # Round score
        totalProblemScore = round(totalProblemScore * 100) / 100
        totalProblemScale = round(totalProblemScale * 100) / 100

        if problemInvalidated:
            totalProblemScore = 0
            bonusProblemScore = 0
            if totalProblemScale > 0:
                row = ['TOTAL', 'FAILED']
                print("{: <65} {: >11}".format(*row))
            if bonusProblemScale > 0:
                row = ['BONUS', 'FAILED']
                print("{: <65} {: >11}".format(*row))
        else:
            if totalProblemScale > 0:
                row = ['TOTAL', str(totalProblemScore), "/" + str(totalProblemScale)]
                print("{: <65} {: >5} {: >5}".format(*row))
            if bonusProblemScale > 0:
                row = ['BONUS', "+" + str(bonusProblemScore), "/" + str(bonusProblemScale)]
                print("{: <65} {: >5} {: >5}".format(*row))
        print("")

        totalScore = totalScore + totalProblemScore
        totalScale = totalScale + totalProblemScale
        bonusScore = bonusScore + bonusProblemScore
        bonusScale = bonusScale + bonusProblemScale
        scoresJSON['scores'][problem] = totalProblemScore + bonusProblemScore
    
    print("------------------------------------------------------------------------------")
    row = ['Total score', str(totalScore), "/" + str(totalScale)]
    print("{: <65} {: >5} {: >5}".format(*row))
    if bonusScale > 0:
        row = ['Bonus', "+" + str(bonusScore), "/" + str(bonusScale)]
        print("{: <65} {: >5} {: >5}".format(*row))
        row = ['TOTAL', str(bonusScore + totalScore), "/" + str(totalScale)]
        print("{: <65} {: >5} {: >5}".format(*row))
        
    print("------------------------------------------------------------------------------")
    if not handoutMode:
        print(json.dumps(scoresJSON))


if __name__ == '__main__':
    grade(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5])
