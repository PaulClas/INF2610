# Comm Lab - Grader

import sys
import subprocess
import json

PRINT_ORDER = ['puzzle1', 'puzzle2', 'puzzle3']

SCORES = {
    'puzzle1': {
        'description': 'Scores for puzzle 1',
        '1': {
            'description': 'Managed to open input pipe',
            'granted': False,
            'scale': 0.5
        },
        '2': {
            'description': 'Managed to read from input pipe',
            'granted': False,
            'scale': 0.5
        },
        '3': {
            'description': 'Received token is correct',
            'granted': False,
            'scale': 1.0
        },
        '4': {
            'description': 'Managed to open output pipe',
            'granted': False,
            'scale': 0.5
        },
        '5': {
            'description': 'Received correct signal',
            'granted': False,
            'scale': 1.0
        },
        '6': {
            'description': 'Managed to send message back',
            'granted': False,
            'scale': 0.5
        },
        '7': {
            'description': 'The received message is correct',
            'granted': False,
            'scale': 1.0
        }
    },
    'puzzle2': {
        'description': 'Scores for puzzle 2',
        '1': {
            'description': 'Valid file descriptor given',
            'granted': False,
            'scale': 1.5
        },
        '2': {
            'description': 'Managed to receive message',
            'granted': False,
            'scale': 1.5
        },
        '3': {
            'description': 'Received message is correct',
            'granted': False,
            'scale': 2.0
        }
    },
    'puzzle3': {
        'description': 'Scores for puzzle 3',
        '0_exc1_launched': {
            'description': 'Executable exc1 launched once',
            'passed': False,
            'countEvents': 0
        },
        '0_exc2_launched': {
            'description': 'Executable exc2 launched once',
            'passed': False,
            'countEvents': 0
        },
        '0_exc3_launched': {
            'description': 'Executable exc3 launched once',
            'passed': False,
            'countEvents': 0
        },
        '0_exc4_launched': {
            'description': 'Executable exc4 launched once',
            'passed': False,
            'countEvents': 0
        },
        '0_no_open': {
            'description': 'No file creation in students\' code',
            'passed': True
        },
        '0_no_sleep': {
            'description': 'No sleep in students\' code',
            'passed': True
        },
        '0_no_write': {
            'description': 'No printf or write from students\' code',
            'passed': True
        },
        '0_no_system': {
            'description': 'No call to the "system" library function',
            'passed': True
        },
        '1_1_stdin': {
            'description': 'STDIN connections are correct',
            'score': 0.0,
            'unitScale': 2.0,
            'scale': 4.0
        },
        '1_2_output': {
            'description': 'The global output is correct',
            'granted': False,
            'scale': 2.0
        }
    }
}

P3_INFO = {
    'rootPIDs': []
}

def handleReportLine(reportLine):
    key, token = reportLine.split(": ")

    if key.startswith('root_'):
        if key == 'root_3':
            P3_INFO['rootPIDs'].append(token)
        return
    
    if 'countEvents' in SCORES[key][token]:
        SCORES[key][token]['countEvents'] = SCORES[key][token]['countEvents'] + 1
    elif 'granted' in SCORES[key][token]:
        SCORES[key][token]['granted'] = True
    elif 'unitScale' in SCORES[key][token]:
        SCORES[key][token]['score'] = SCORES[key][token]['score'] + SCORES[key][token]['unitScale']

def gradeP3Constraints(p3StraceLogLines):
    # Check events count
    for _, obj in SCORES['puzzle3'].items():
        if 'countEvents' in obj:
            if obj['countEvents'] == 1:
                obj['passed'] = True

    # Check system calls in strace output
    
    legitimatePids = {}
    for pid in P3_INFO['rootPIDs']:
        legitimatePids[pid] = 0

    for i in range(len(p3StraceLogLines)):
        pid, info = p3StraceLogLines[i].split(None, 1)
        if info.find('exec') != -1:
            if info.find('puzzle3/exc') != -1:
                legitimatePids[pid] = i
    for i in range(len(p3StraceLogLines)):
        try:
            pid, info = p3StraceLogLines[i].split(None, 1)
            if info.find('openat(') != -1:
                if info.find('puzzle3/tmpfile') == -1 and info.find('... openat resumed') == -1:
                    if not pid in legitimatePids or i < legitimatePids[pid]:
                        SCORES['puzzle3']['0_no_open']['passed'] = False
            if info.find('write') != -1:
                if not pid in legitimatePids or i < legitimatePids[pid]:
                    # If not the root process
                    if not pid in legitimatePids or legitimatePids[pid] > 0:
                        SCORES['puzzle3']['0_no_write']['passed'] = False
            if info.find('nanosleep') != -1:
                if not pid in legitimatePids or i < legitimatePids[pid]:
                    SCORES['puzzle3']['0_no_sleep']['passed'] = False
            if info.find('execve("/bin/') != -1:
                if not pid in legitimatePids or i < legitimatePids[pid]:
                    SCORES['puzzle3']['0_no_system']['passed'] = False
        except KeyError:
            continue

def grade(gradeLogPath, p3StraceLogPath, mode):
    handoutMode = True if mode == 'handout' else False
    with open(gradeLogPath, "r") as gradeLogFile:
        lines = gradeLogFile.readlines()

    for l in lines:
        handleReportLine(l.rstrip())
    
    # Special handling puzzle 3
    with open(p3StraceLogPath, "r") as p3StraceLogFile:
        p3StraceLogLines = p3StraceLogFile.readlines()
    gradeP3Constraints(p3StraceLogLines)

    print("--------------------")
    print("Comm Lab: Autograder")
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
        print("---------------------------------------------------------------")
        
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
                print("{: <50} {: >11}".format(*row))
            else:
                if 'isBonus' in subscores and subscores['isBonus']:
                    row = ['-> ' + subscores['description'], "+" + str(subscore), "/" + str(subscores['scale'])]
                    print("{: <50} {: >5} {: >5}".format(*row))
                else:
                    row = ['-> ' + subscores['description'], str(subscore), "/" + str(subscores['scale'])]
                    print("{: <50} {: >5} {: >5}".format(*row))

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
                print("{: <50} {: >11}".format(*row))
            if bonusProblemScale > 0:
                row = ['BONUS', 'FAILED']
                print("{: <50} {: >11}".format(*row))
        else:
            if totalProblemScale > 0:
                row = ['TOTAL', str(totalProblemScore), "/" + str(totalProblemScale)]
                print("{: <50} {: >5} {: >5}".format(*row))
            if bonusProblemScale > 0:
                row = ['BONUS', "+" + str(bonusProblemScore), "/" + str(bonusProblemScale)]
                print("{: <50} {: >5} {: >5}".format(*row))
        print("")

        totalScore = totalScore + totalProblemScore
        totalScale = totalScale + totalProblemScale
        bonusScore = bonusScore + bonusProblemScore
        bonusScale = bonusScale + bonusProblemScale
        scoresJSON['scores'][problem] = totalProblemScore + bonusProblemScore
    
    print("---------------------------------------------------------------")
    row = ['Total score', str(totalScore), "/" + str(totalScale)]
    print("{: <50} {: >5} {: >5}".format(*row))
    if bonusScale > 0:
        row = ['Bonus', "+" + str(bonusScore), "/" + str(bonusScale)]
        print("{: <50} {: >5} {: >5}".format(*row))
        row = ['TOTAL', str(bonusScore + totalScore), "/" + str(totalScale)]
        print("{: <50} {: >5} {: >5}".format(*row))
        
    print("---------------------------------------------------------------")
    if not handoutMode:
        print(json.dumps(scoresJSON))


if __name__ == '__main__':
    grade(sys.argv[1], sys.argv[2], sys.argv[3])