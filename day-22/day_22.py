"""
    @file day_22.py
    @brief day 22 advent of code solution.
    @author Graham Riches
    @details got really tired of string parsing in C++ so started switching over to doing these in python
             unless I'm feeling particularly aggressive :D
"""
import re


def load_player_hands(filename: str) -> tuple:
    """
    load the player hands from a file
    :param filename: the filename
    :return: tuple of hands (p1, p2)
    """
    player_hands = re.sub('Player \\d:\n', '', open(filename).read())
    p1, p2 = player_hands.split('\n\n')
    player_one = [int(x) for x in p1.split('\n')]
    player_two = [int(x) for x in p2.split('\n')]
    return player_one, player_two


def regular_combat(p1: list, p2: list) -> list:
    """
    play a regular game of Combat between p1 and p2. Return the winning hand
    :param p1: player one hand
    :param p2: player two hand
    :return:the winning hand
    """
    while len(p1) != 0 and len(p2) != 0:
        cards = [p1.pop(0), p2.pop(0)]
        if cards[0] > cards[1]:
            p1.extend(cards)
        else:
            p2.extend(sorted(cards, reverse=True))
    return p1 if len(p1) > len(p2) else p2


def recursive_combat(p1: list, p2: list) -> tuple:
    """
    play a game of recursive combat. Note: this returns a tuple containing each players hand
    after the game. This is used recursively to figure out who won sub rounds/
    :param p1: player one's hand
    :param p2: player two's hand
    :return: each players hands
    """
    game_hands = dict()
    iteration = 0
    while len(p1) != 0 and len(p2) != 0:
        p1_start = p1[:]
        p2_start = p2[:]
        for key, value in game_hands.items():
            if value[0] == p1_start and value[1] == p2_start:
                return [1], []
        else:
            cards = [p1.pop(0), p2.pop(0)]
            # game recursion condition
            if len(p1) >= cards[0] and len(p2) >= cards[1]:
                r1, r2 = recursive_combat(p1[0:cards[0]], p2[0:cards[1]])
                # check for duplicate returns
                if r1 == [1] and r2 == []:
                    p1.extend(cards)
                elif len(r1) > len(r2):
                    p1.extend(cards)
                else:
                    cards.reverse()
                    p2.extend(cards)
            elif cards[0] > cards[1]:
                p1.extend(cards)
            else:
                p2.extend(sorted(cards, reverse=True))
        game_hands[iteration] = [p1_start, p2_start]
        iteration += 1
    return p1, p2


def calculate_score(winning_hand: list) -> int:
    """
    calculate the score of the winning hand
    :param winning_hand: list of cards
    :return: hand score
    """
    return sum([x * y for x, y in zip(winning_hand, range(len(winning_hand), 0, -1))])


if __name__ == '__main__':
    # Part One
    score = calculate_score(regular_combat(*load_player_hands('input.txt')))
    print('Winning Score: {}'.format(score))

    # Part Two
    p1, p2 = recursive_combat(*load_player_hands('input.txt'))
    winner = p1 if len(p1) > len(p2) else p2
    score = calculate_score(winner)
    print('Winning Score: {}'.format(score))
