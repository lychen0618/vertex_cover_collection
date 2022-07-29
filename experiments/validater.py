# /usr/bin/python
# -*- encoding=utf-8 -*-

import os
import argparse
from io import open


def read_one_file(file_handler, line):
    dataset = line.strip().split(' ')[-2]
    file_name = file_handler.readline().strip().split(' ')[-2]
    line = file_handler.readline()
    vertex_num = line.strip().split(' ')[-3]
    edge_num = line.strip().split(' ')[-1]

    vertex_cover_list = list()
    string_list = list()
    while True:
        line = file_handler.readline()
        if line == '' or '#### Process Dataset' in line or 'time cost' in line:
            break
        one_vc_list = [int(v) for v in line.strip().split(' ')[7:]]
        one_vc_list.sort()
        vertex_cover_list.append(one_vc_list)
        string_list.append(' '.join([str(iv) for iv in one_vc_list]))

    time_cost = line.strip().split(' ')[-1]
    line = file_handler.readline()

    vertex_cover_list.sort()
    for id in range(1, len(vertex_cover_list)):
        if vertex_cover_list[id] == vertex_cover_list[id - 1]:
            print('error: duplicate vertex covers')

    print('dataset[{}] file[{}] vertex[{}] edge[{}] vertex_cover[{}] time_cost[{}]'.format(
        dataset, file_name, vertex_num, edge_num, len(vertex_cover_list), time_cost))
    return line, string_list


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--log_file', type=str, required=True)
    args = parser.parse_args()
    log_dir = os.path.join(os.path.split(os.path.realpath(__file__))[0], 'log')
    with open(os.path.join(log_dir, args.log_file), 'r', encoding='utf-8') as f:
        while True:
            line = f.readline()
            if line == '' or '#### Process Dataset' in line:
                break
        line, vcl1 = read_one_file(f, line)
        _, vcl2 = read_one_file(f, line)
        if len(vcl1) != len(vcl2):
            print('error: number of vertex covers not equal')
            return

        vcl2_set = set(vcl2)
        for vc in vcl1:
            if vc not in vcl2_set:
                print('error: not identical result')
                return

        print('validate success')


if __name__ == '__main__':
    main()
