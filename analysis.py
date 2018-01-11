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

print 'a: %d' % test.count('a')
print 'b: %d' % test.count('b')
print 'c: %d' % test.count('c')

start = time.time()
print(editdistance.eval(org, test))
elapsed_time = time.time() - start

print("elapsed_time:{0}".format(elapsed_time) + "[sec]")
