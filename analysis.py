import editdistance
import time
import sys


args = sys.argv

if len(args) != 3:
    print('argument error')
    print('$ analysis.py <test_filename> <org_filename>')
    quit()

test_file = open(args[1], 'r')
org_file = open(args[2], 'r')
test = test_file.read()
org = org_file.read()

print '-----------------------------'
print '|   |   test    |    org    |'
print '| a |   %d   |   %d   |' % (test.count('a'), org.count('a'))
print '| b |   %d   |   %d   |' % (test.count('b'), org.count('b'))
print '| c |   %d   |   %d   |' % (test.count('c'), org.count('c'))
print '-----------------------------'
print '| Edit Distance |   %d   |' % (editdistance.eval(org, test))
print '-----------------------------'
