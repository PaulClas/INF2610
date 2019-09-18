# Init Lab - Grader

import sys
import subprocess
import json
from yaml import load, Loader
from difflib import SequenceMatcher
import math

PRINT_ORDER = ['q1', 'q2', 'q3', 'q4', 'q5']

SCORES = {
    'q1': {
        'description': 'Scores for question 1',
        'test1': {
            'description': 'Test 1 (1 argument) succeeded',
            'granted': False,
            'scale': 0.5,
        },
        'test2': {
            'description': 'Test 2 (2 arguments) succeeded',
            'granted': False,
            'scale': 0.5,
        },
        'test3': {
            'description': 'Test 3 (4 arguments) succeeded',
            'granted': False,
            'scale': 0.5,
        },
        'test4': {
            'description': 'Test 4 (4 arguments) succeeded',
            'granted': False,
            'scale': 0.5,
        }
    },
    'q2': {
        'description': 'Scores for question 2',
        'output': {
            'description': 'The standard output message is correct',
            'granted': False,
            'scale': 2.0,
        },
        'printfBeforeWrite': {
            'description': 'One correct printf call was used before the write syscall',
            'granted': False,
            'scale': 1.0,
        },
        'write': {
            'description': 'At least one correct write syscall is used',
            'passed': False
        }
    },
    'q3': {
        'description': 'Scores for question 3',
        'nonempty': {
            'description': 'The file was not found empty',
            'granted': False,
            'scale': 1.0,
        },
        'output': {
            'description': 'The message written in the file is correct',
            'granted': False,
            'scale': 1.0,
        }
    },
    'q4': {
        'description': 'Scores for question 4',
        'A': {
            'description': 'Function A was correctly implemented',
            'granted': False,
            'scale': 0.5,
        },
        'B': {
            'description': 'Function B was correctly implemented',
            'granted': False,
            'scale': 0.5,
        },
        'C': {
            'description': 'Function C was correctly implemented',
            'granted': False,
            'scale': 1.5,
        },
        'D': {
            'description': 'Function D was correctly implemented',
            'granted': False,
            'scale': 1.5,
        },
        'E': {
            'description': 'Function E was correctly implemented',
            'granted': False,
            'scale': 1.5,
        }
    },
    'q5': {
        'description': 'Scores for question 5',
        '5A': {
            'description': 'Answer for question 5 a) is correct',
            'granted': False,
            'scale': 0.5
        },
        '5B': {
            'description': 'Answer for question 5 b) is correct',
            'granted': False,
            'scale': 1.0
        },
        '5C': {
            'description': 'Answer for question 5 c) is correct',
            'granted': False,
            'scale': 0.5
        },
        '5D': {
            'description': 'Answer for question 5 d) is correct',
            'granted': False,
            'scale': 0.5
        },
        '5E': {
            'description': 'Answer for question 5 e) is correct',
            'granted': False,
            'scale': 1.0
        }
    }
}

PARAMS = {}
PART1_PROCESSTREE = {}

def handleReportLine(reportLine):
    key, token = reportLine.split(": ")

    if 'countEvents' in SCORES[key][token]:
        SCORES[key][token]['countEvents'] = SCORES[key][token]['countEvents'] + 1
    elif 'granted' in SCORES[key][token]:
        SCORES[key][token]['granted'] = True
    elif 'unitScale' in SCORES[key][token]:
        SCORES[key][token]['score'] = SCORES[key][token]['score'] + SCORES[key][token]['unitScale']

def handleQ2LtraceOutput(lines):
    rootPid = 0
    questionPid = 0
    writeCalled = False
    writePending = False
    printfPending = False

    for s in lines:
        try:
            spl = s.split()
            if int(spl[0]) == questionPid:
                spl2 = s.split(' ', 1)
                if spl2[1].startswith('printf("') and spl2[1].find(' (printed using printf)') != -1:
                    if spl2[1].find('unfinished') != -1:
                        printfPending = True
                    else:
                        if int(spl[-1]) > 0 and not writeCalled:
                            SCORES['q2']['printfBeforeWrite']['granted'] = True
                    continue
                if spl2[1].find('printf resumed') != -1:
                    if printfPending:
                        if int(spl[-1]) > 0:
                            if not writeCalled:
                                SCORES['q2']['printfBeforeWrite']['granted'] = True
                    continue
                if spl2[1].startswith('write(1, '):
                    if spl2[1].find('unfinished') == -1 and int(spl[-1]) > 0:
                        writeCalled = True
                        SCORES['q2']['write']['passed'] = True
                    else:
                        writePending = True
                    continue
                if spl2[1].find('write resumed') != -1:
                    if writePending:
                        if int(spl[-1]) > 0:
                            writeCalled = True
                            SCORES['q2']['write']['passed'] = True
                        writePending = False
            elif int(spl[0]) == rootPid:
                spl2 = s.split(' ', 1)
                if spl2[1].find('fork(') != -1 or spl2[1].find('fork resumed') != -1:
                    if len(spl) > 1 and spl[-2] == '=':
                        if questionPid == 0:
                            questionPid = int(spl[-1])
                continue
            else:
                spl2 = s.split(' ', 1)
                if spl2[1].find('fork(') != -1 or spl2[1].find('fork resumed') != -1:
                    if len(spl) > 1 and spl[-2] == '=':
                        if rootPid == 0:
                            rootPid = int(spl[-1])
        except ValueError:
            continue

def grade(gradeLogPath, paramsFilePath, q2LtraceLogPath, mode):
    handoutMode = True if mode == 'handout' else False

    with open(gradeLogPath, "r") as gradeLogFile:
        lines = gradeLogFile.readlines()

    for l in lines:
        handleReportLine(l.rstrip())

    # Handle question 2
    with open(q2LtraceLogPath, "r") as q2LtraceLogFile:
        lines = q2LtraceLogFile.readlines()
        handleQ2LtraceOutput(lines)

    print("--------------------")
    print("Init Lab: Autograder")
    print("--------------------")
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
    grade(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4])
