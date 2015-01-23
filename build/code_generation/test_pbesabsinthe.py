#!/usr/bin/env python

#~ Copyright 2012 Wieger Wesselink.
#~ Distributed under the Boost Software License, Version 1.0.
#~ (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

from path import *
from random_pbes_generator import *
from mcrl2_tools import *

def test_pbesabsinthe(p, filename, strategy):
    txtfile = filename + '.txt'
    path(txtfile).write_text('%s' % p)
    pbesfile1 = filename + 'a.pbes'
    pbesfile2 = filename + 'b.pbes'
    run_txt2pbes(txtfile, pbesfile1)
    run_pbesabsinthe(pbesfile1, pbesfile2, strategy)
    answer1 = run_pbes2bool(pbesfile1)
    answer2 = run_pbes2bool(pbesfile2)
    print filename, answer1, answer2
    if answer1 == None or answer2 == None:
      return True
    return answer1 == answer2

def main():
    options = parse_command_line()
    try:
        equation_count = 2
        atom_count = 2
        propvar_count = 2
        use_quantifiers = True

        for i in range(options.iterations):
            filename = 'pbesabsinthe'
            p = make_pbes(equation_count, atom_count, propvar_count, use_quantifiers)
            if i % 2 == 0:
                strategy = 'over'
            else:
                strategy = 'under'
            if not test_pbesabsinthe(p, filename, strategy):
                m = CounterExampleMinimizer(p, lambda x: test_pbesabsinthe(x, filename + '_minimize', strategy), 'pbesabsinthe')
                m.minimize()
                raise Exception('Test %s.txt failed' % filename)
    finally:
        if not options.keep_files:
            remove_temporary_files()

if __name__ == '__main__':
    main()