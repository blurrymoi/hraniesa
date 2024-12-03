from common import *
from collections import Counter

# locations have location IDs

if __name__ == '__main__':
    f = load_file('aoc1_1')

    # smallest with smallest, second smallest with second smallest
    nums = [line for line in f.read().split('\n') if line]
    nums_split = map(lambda x: map(int, filter(None, x.split(' '))), nums)
    list1, list2 = zip(*nums_split)
    x = tuple(map(lambda i, j: abs(i - j), sorted(list1), sorted(list2)))
    print(sum(x), x)

    f.close()

    # second part -- similarity score
    list2_counter = Counter(list2)
    print(list2_counter)
    occurences = [x * list2_counter[x] for x in list1]
    print(sum(occurences), occurences)
