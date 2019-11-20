# Mem Lab - Grader

import sys
import subprocess
import json
from yaml import load, Loader
import math

PRINT_ORDER = ['memsim']

SCORES = {
    'memsim': {
        'description': 'Scores for question 1',
        'test0_1': {
            'description': 'Function getPageOffset is correct',
            'granted': False,
            'scale': 1.0
        },
        'test0_2': {
            'description': 'Function getPageNumber is correct',
            'granted': False,
            'scale': 1.0
        },
        'test0_3': {
            'description': 'Function getStartPageAddress is correct',
            'granted': False,
            'scale': 1.0
        },
        'test1_1': {
            'description': 'Test case 1: Correctness of physical addresses',
            'score': 0.0,
            'scale': 1.0
        },
        'test1_2': {
            'description': 'Test case 1: Correctness of checks in TLB or page table',
            'score': 0.0,
            'scale': 1.0
        },
        'test1_3': {
            'description': 'Test case 1: Correctness of TLB updates',
            'score': 0.0,
            'scale': 1.0
        },
        'test1_4': {
            'description': 'Test case 1: Correctness of page table updates',
            'score': 0.0,
            'scale': 1.0
        },
        'test2_1': {
            'description': 'Test case 2: Correctness of physical addresses',
            'score': 0.0,
            'scale': 0.8
        },
        'test2_2': {
            'description': 'Test case 2: Correctness of checks in TLB or page table',
            'score': 0.0,
            'scale': 0.8
        },
        'test2_3': {
            'description': 'Test case 2: Correctness of TLB updates',
            'score': 0.0,
            'scale': 0.7
        },
        'test2_4': {
            'description': 'Test case 2: Correctness of page table updates',
            'score': 0.0,
            'scale': 0.7
        },
        'test3_1': {
            'description': 'Test case 3: Correctness of physical addresses',
            'score': 0.0,
            'scale': 0.8
        },
        'test3_2': {
            'description': 'Test case 3: Correctness of checks in TLB or page table',
            'score': 0.0,
            'scale': 0.8
        },
        'test3_3': {
            'description': 'Test case 3: Correctness of TLB updates',
            'score': 0.0,
            'scale': 0.7
        },
        'test3_4': {
            'description': 'Test case 3: Correctness of page table updates',
            'score': 0.0,
            'scale': 0.7
        },
        'test4_1': {
            'description': 'Test case 4: Correctness of physical addresses',
            'score': 0.0,
            'scale': 0.8
        },
        'test4_2': {
            'description': 'Test case 4: Correctness of checks in TLB or page table',
            'score': 0.0,
            'scale': 0.8
        },
        'test4_3': {
            'description': 'Test case 4: Correctness of TLB updates',
            'score': 0.0,
            'scale': 0.7
        },
        'test4_4': {
            'description': 'Test case 4: Correctness of page table updates',
            'score': 0.0,
            'scale': 0.7
        },
    },
}

PARAMS = {}

def handleReportLine(reportLine):
    key, token = reportLine.split(": ")

    if key in ['memsim/testCase1.txt', 'memsim/testCase2.txt', 'memsim/testCase3.txt', 'memsim/testCase4.txt']:
        testIndex = key[-5:-4]
        scoresKey = 'test' + testIndex + '_'

        scores = [int(count) for count in token.split(', ')]
        totalEvents = scores[0]
        correctFrameNumbers = scores[1]
        correctOffsets = scores[2]
        correctChecks = scores[3]
        correctTLBUpdates = scores[4]
        correctPTUpdates = scores[5]

        score_1 = (3 * correctFrameNumbers + correctOffsets) / (4 * totalEvents)
        score_1 = math.ceil(10 * score_1 * SCORES['memsim'][scoresKey + '1']['scale']) / 10
        SCORES['memsim'][scoresKey + '1']['score'] = score_1
        score_2 = correctChecks / totalEvents
        score_2 = math.ceil(10 * score_2 * SCORES['memsim'][scoresKey + '2']['scale']) / 10
        SCORES['memsim'][scoresKey + '2']['score'] = score_2
        score_3 = correctTLBUpdates / totalEvents
        score_3 = math.ceil(10 * score_3 * SCORES['memsim'][scoresKey + '3']['scale']) / 10
        SCORES['memsim'][scoresKey + '3']['score'] = score_3
        score_4 = correctPTUpdates / totalEvents
        score_4 = math.ceil(10 * score_4 * SCORES['memsim'][scoresKey + '4']['scale']) / 10
        SCORES['memsim'][scoresKey + '4']['score'] = score_4
        return

    if 'countEvents' in SCORES[key][token]:
        SCORES[key][token]['countEvents'] = SCORES[key][token]['countEvents'] + 1
    elif 'granted' in SCORES[key][token]:
        SCORES[key][token]['granted'] = True
    elif 'unitScale' in SCORES[key][token]:
        SCORES[key][token]['score'] = SCORES[key][token]['score'] + SCORES[key][token]['unitScale']

def grade(gradeLogPath, mode):
    handoutMode = True if mode == 'handout' else False

    with open(gradeLogPath, "r") as gradeLogFile:
        lines = gradeLogFile.readlines()

    for l in lines:
        handleReportLine(l.rstrip())

    print("-------------------")
    print("Mem Lab: Autograder")
    print("-------------------")
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
            row = ['TOTAL', 'FAILED']
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
    grade(sys.argv[1], sys.argv[2])
