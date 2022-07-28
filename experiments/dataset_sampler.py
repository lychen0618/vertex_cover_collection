# /usr/bin/python
# -*- encoding=utf-8 -*-

import argparse
import os
import random
from io import open


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--input_path', type=str, required=True)
    parser.add_argument('--output_dir', type=str, required=True)
    parser.add_argument('--rate', type=str, required=True,
                        help='sample rate of edge')
    args = parser.parse_args()
    rate_list = [float(r) for r in args.rate.strip().split(',')]
    with open(args.input_path, 'r', encoding='utf-8') as whole_dataset:
        lines = whole_dataset.readlines()
        if lines[-1] == '\n':
            del lines[-1]
        if not os.path.exists(args.output_dir):
            os.mkdir(args.output_dir)
        for rate in rate_list:
            num = int(rate * len(lines))
            result_list = random.sample(range(0, len(lines)), num)
            num_lines = []
            for i in result_list:
                num_lines.append([int(v) for v in lines[i].split(' ')])
            # calculate vertex num
            vertex_set = set()
            for line in num_lines:
                for v in line:
                    vertex_set.add(v)
            # new_v_map = map()
            # id = 0
            # for old_v in vertex_set:
            #     new_v_map[old_v] = id
            #     id += 1
            with open(os.path.join(args.output_dir,
                                   'v{}_e{}.txt'.format(len(vertex_set), num)),
                      'w+', encoding='utf-8') as f:
                for i in result_list:
                    f.write(lines[i])


if __name__ == '__main__':
    main()
