"""
    @file day_25.py
    @brief final AoC 2020 solution
    @author Graham Riches
    @details
   
"""


def get_loop_size(key: int, initial_value: int) -> int:
    """
    calculate the loop size of a key
    :param key:
    :param initial_value:
    :return:
    """
    loops = 0
    new_key = 1
    while new_key != key:
        new_key = (new_key * initial_value) % 20201227
        loops += 1
    return loops


def calculate_key(key: int, loops: int) -> int:
    """
    calculate a new key value
    :param key:
    :param loops:
    :return:
    """
    new_key = 1
    for i in range(loops):
         new_key = (new_key * key) % 20201227
    return new_key


if __name__ == '__main__':
    inputs = [int(x.strip('\n')) for x in open('input.txt').readlines()]
    loops = [get_loop_size(x, 7) for x in inputs]
    key = calculate_key(inputs[0], loops[1])
    print('Key: {}'.format(key))
