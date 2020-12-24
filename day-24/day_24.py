"""
    @file day_24.py
    @brief advent of code day 24 solution
    @author Graham Riches
    @details Hexagonal tile game of life type problem with some annoying string parsing at the beginning. Pretty
             simple overall.
"""


def parse_line(line: str) -> list:
    """
    process an input string into hex tile instructions
    :param line: the line to parse
    :return: list of instructions
    """
    instructions = list()
    compound_directions = ['ne', 'nw', 'se', 'sw']
    count = 0
    while count < len(line):
        if line[count:count+2] in compound_directions:
            instructions.append(line[count:count+2])
            count += 2
        else:
            instructions.append(line[count])
            count += 1
    return instructions


def get_target_tile(instructions: list) -> tuple:
    """
    get the target tile from a list of instructions
    :param instructions: list of instructions
    :return: tuple of coordinates
    """
    coordinates = (0, 0)
    transformation_map = {'e': (0, 1), 'se': (1, 0), 'sw': (1, -1), 'w': (0, -1), 'nw': (-1, 0), 'ne': (-1, 1)}
    for instruction in instructions:
        coordinates = tuple(map(sum, zip(coordinates, transformation_map[instruction])))
    return coordinates


def get_neighbour_tiles(tile: tuple) -> list:
    """
    get a list of adjacent tiles to a tile
    :param tile: tile coordinates
    :return: list of adjacent tiles
    """
    transformation_map = {'e': (0, 1), 'se': (1, 0), 'sw': (1, -1), 'w': (0, -1), 'nw': (-1, 0), 'ne': (-1, 1)}
    return [tuple(map(sum, zip(tile, value))) for key, value in transformation_map.items()]


def tile_game_of_life(tiles: dict) -> dict:
    """
    apply the game of life tile rules to the hex grid
    :param tiles: dictionary of tiles
    :return: new dictionary of tiles
    """
    new_tiles = dict()
    black_tiles = {key: True for key, value in tiles.items() if value is True}
    white_tiles = {key: True for key, value in tiles.items() if value is False}

    # for each black tile, also append any new white neighbours to the white tile set to be checked afterwards
    for tile, _ in black_tiles.items():
        neighbours = get_neighbour_tiles(tile)
        white_tiles.update({neighbour: False for neighbour in neighbours if neighbour not in tiles.keys()})
        adjacent_black = sum([1 for neighbour in neighbours if neighbour in black_tiles.keys()])
        new_tiles[tile] = False if (adjacent_black == 0 or adjacent_black > 2) else True
    for tile, _ in white_tiles.items():
        neighbours = get_neighbour_tiles(tile)
        adjacent_black = sum([1 for neighbour in neighbours if neighbour in black_tiles.keys()])
        new_tiles[tile] = (adjacent_black == 2)
    return new_tiles


if __name__ == '__main__':
    """ Part One Solution """
    tiles = dict()
    for line in open('input.txt').readlines():
        target_tile = get_target_tile(parse_line(line.strip('\n')))
        tiles[target_tile] = not tiles[target_tile] if target_tile in tiles.keys() else True
    print('number of black tiles: {}'.format(sum([value for key, value in tiles.items() if value is True])))

    """ Part Two Solution """
    for i in range(100):
        tiles = tile_game_of_life(tiles)
    print('number of black tiles: {}'.format(sum([value for key, value in tiles.items() if value is True])))
