#!/usr/bin/python
import sys

# for n in {400,800,1600,3200}; do; for alg in {s,t,a,d}; do; cat ex-problem-0-$alg-n-$n-run-1.out | grep "^?" | xargs ../../lineToTab.py > ex-problem-0-$alg-n-$n-run-1.line ; done; done
# for n in {400,800,1600,3200}; do; for alg in {s,t,a,d}; do; cat ex-problem-1-$alg-n-$n-run-1.out | grep "^?" | xargs ../../lineToTab.py > ex-problem-1-$alg-n-$n-run-1.line ; done; done
# for n in {400,800,1600,3200}; do; for alg in {s,t,a,d}; do; cat ex-problem-2-$alg-n-$n-run-1.out | grep "^?" | xargs ../../lineToTab.py > ex-problem-2-$alg-n-$n-run-1.line ; done; done
# for n in {400,800,1600,3200}; do; for alg in {s,t,a,d}; do; cat ex-problem-3-$alg-n-$n-run-1.out | grep "^?" | xargs ../../lineToTab.py > ex-problem-3-$alg-n-$n-run-1.line ; done; done
# for n in {400,800,1600,3200}; do; for alg in {s,t,a,d}; do; cat ex-problem-4-$alg-n-$n-run-1.out | grep "^?" | xargs ../../lineToTab.py > ex-problem-4-$alg-n-$n-run-1.line ; done; done

def lineToTab(inp):
    if inp[0] == '?':
        inp = inp[1:]
    print "\n".join(["{}\t{}".format(x[1:], y[:-1]) for (x, y) in [x.split(',') for x in inp.split('->')]])


def main(argv):
    if len(argv) < 1:
        print "Please provide line string in form \"(x1,y1)->(x2,y2)->...\""
        exit(1)

    lineToTab(argv[0])

if __name__ == "__main__":
    # print sys.argv
    main(sys.argv[1:])
