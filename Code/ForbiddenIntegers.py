def readForbiddenIntegers():
    with open("forbidden_integers.txt") as f:
        forbidden_integers_list = f.readlines()

    forbidden_integers_list = [x.strip().split('-') for x in sorted(forbidden_integers_list)]

    print len(forbidden_integers_list)

    new_list = []

    new_list.append(forbidden_integers_list[0])

    j = 0;

    for i in xrange(1,len(forbidden_integers_list)):
        if new_list[j][1] <= forbidden_integers_list[i][0]:
            new_list[j][1] = forbidden_integers_list[i][1]
        else:
            new_list.append(forbidden_integers_list[i])
            j += 1

    print len(new_list)

    count = 4294967295;

    for i in new_list:
        count -= long(i[1])-long(i[0])

    print 'Count of  allowed Integers: ' + str(count)

    min = 0
    if new_list[0][0] == '0':
        min = long(new_list[0][1]) + 1

    print 'Min allowed Integers: ' + str(min)

def main ():
    readForbiddenIntegers()

if __name__ == "__main__":
    main()