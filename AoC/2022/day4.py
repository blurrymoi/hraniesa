
overlaps = 0
do_not_overlap = 0
pairs = 0


def beg_end(interval):
    return list(map(int, interval.strip().split('-')))


for line in open("input4.txt"):

    if not line.strip():
        continue

    section1,section2 = line.strip().split(',')
    s1_1, s1_2 = beg_end(section1)
    s2_1, s2_2 = beg_end(section2)
    pairs += 1

    if s1_2 > s2_2 or (s1_2 == s2_2 and s2_1 > s1_1):
        # s2 now always bigger or equal to s1 (or bigger range if equal end)
        s1_1, s2_1 = s2_1, s1_1
        s1_2, s2_2 = s2_2, s1_2
    # if s1 fully contained in s2
    if s1_1 >= s2_1 and s1_2 <= s2_2:
        print("overlaps:")
        print(s1_1, s1_2)
        print(s2_1, s2_2)
        overlaps += 1
    elif s1_2 < s2_1:
        do_not_overlap += 1


print("number of overlaps:", overlaps)
print("do not overlap:", do_not_overlap)
print("pairs:", pairs)
print("pairs - do not overlap:", pairs - do_not_overlap)

