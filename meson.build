project('life', ['cpp'])

check = dependency('check')
sdl2 = dependency('sdl2')
args = ['-pedantic', '-std=c++11']

t = executable('test_game', ['test_life.cpp', 'life.cpp'], cpp_args:args, dependencies: [check])

executable('game_cli', ['game_cli.cpp', 'life.cpp'], cpp_args: args)
executable('game_sdl', ['game_sdl.cpp' ,'life.cpp'], cpp_args: args, dependencies: [sdl2])

test('basic tests ', t)
