import sys


def find_max_index(permutation):
    for i in range(len(permutation) - 2, -1, -1):
        if permutation[i] < permutation[i+1]:
            return i
    return -1


def find_index_bigger_element(permutation, element):
    for i in range(len(permutation) - 1, -1, -1):
        if permutation[i] > element:
            return i
    return -1


def swap(permutation, i, j):
    permutation[i], permutation[j] = permutation[j], permutation[i]


def reverse_permutation(permutation, index):
    n = len(permutation)
    permutation = permutation[:index+1:] + permutation[n - 1:index:-1]
    return permutation


def permutation_generator(permutation, n):
    print_list(permutation)
    index = find_max_index(permutation)

    while index != -1:
        element = permutation[index]
        swap_index = find_index_bigger_element(permutation, element)
        swap(permutation, index, swap_index)
        permutation = reverse_permutation(permutation, index)
        print_list(permutation)
        index = find_max_index(permutation)

        if n is not None:
            n -= 1
            if n <= 0:
                break

def permutation_generator_range(n, k):
    permutation_generator(list(range(1, n+1)), k)

def print_list(ls):
    print(" ".join(map(lambda a: str(a), ls)))


av = sys.argv[1:]   # аргументы командной строки
n = None            # количество перестановок
permutation = []    # начальная перестановка

# идём по аргументам командной строки
for arg in av:
    if arg == '--help':
        print("""
# Генерировать последовательность из 5 элементов (-5) 6 раз (+6):

    python3 Naraiana_algorithm.py -5 +6

# Задать начальную последовательность (5 4 2 3 1) и прокрутить её 10 раз (+10):

    python3 Naraiana_algorithm.py  +10  5 4 2 3 1
""")
    elif arg[0] == '+':
        n = int(arg[1:])
    elif arg[0] == '-':
        permutation = list(range(1, int(arg[1:])+1))
    else:
        permutation.append(int(arg))


permutation_generator(permutation, n)
