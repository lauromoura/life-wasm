project('life', ['cpp'])

check = dependency('check')
args = ['-pedantic']

t = executable('test_game', ['test_life.cpp', 'life.cpp'], cpp_args:args, dependencies: [check])

executable('game_cli', ['game_cli.cpp', 'life.cpp'], cpp_args: args)

test('basic tests ', t)
