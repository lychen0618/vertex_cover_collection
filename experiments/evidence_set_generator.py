# /usr/bin/python
# -*- encoding=utf-8 -*-

import traceback
import logging
import argparse
import random
import os
import shutil
from io import open

from requests import head
logging.basicConfig(level=logging.DEBUG, filename='./experiments/data/log.tmp', filemode='w+',
                    format='[%(asctime)s][%(levelname)s]: %(message)s', datefmt='%Y-%m-%d %H:%M')


class Pred(object):
    def __init__(self, attr1, attr2, op):
        self.attr1 = attr1
        self.attr2 = attr2
        self.op = op

    def __repr__(self):
        return '{}{}{}'.format(self.attr1, self.op, self.attr2)


class EvidenceSetGenerator(object):
    def __init__(self, input_path, output_path):
        self._input_path = input_path
        self._output_path = output_path
        self._attrs = None
        self._attrs_type = list()
        self._all_str_type = True
        self._preds = list()
        self._tuples = list()
        self._evidence_set = set()

    def _is_compare_op(self, op):
        return ('<' in op or '>' in op)

    def run(self):
        try:
            with open(self._input_path, 'r', encoding='utf-8') as f:
                header = f.readline()
            self._attrs = header.split(',')
            attr_map = dict()
            for idx in range(len(self._attrs)):
                attr = self._attrs[idx]
                attr_type = attr[attr.find('(') + 1: attr.find(')')]
                if attr_type not in attr_map.keys():
                    attr_map[attr_type] = list()
                attr_map[attr_type].append(idx)
                self._attrs_type.append(attr_type[0])
            self._all_str_type = (len(attr_map.keys()) == 1)
            # create predicates
            ops = ['=', '!=', '>=', '>', '<', '<=']
            for attr_type, attr_list in attr_map.items():
                for i in range(len(attr_list)):
                    j = i
                    for op in ops:
                        if attr_type == 'String' \
                                and self._is_compare_op(op):
                            continue
                        self._preds.append(Pred(i, j, op))
            logging.info('predicates: {}'.format(self._preds))
            # read tuples
            with open(self._input_path, 'r', encoding='utf-8') as f:
                f.readline()
                while True:
                    line = f.readline()
                    if line == '':
                        break
                    values = line.strip().split(',')
                    for i in range(len(values)):
                        value = values[i]
                        if value == '':
                            continue
                        if self._attrs_type[i] == 'I':
                            values[i] = int(value.strip())
                        elif self._attrs_type[i] == 'D':
                            values[i] = float(value.strip())
                    self._tuples.append(values)
            # create evidence set
            for i in range(len(self._tuples)):
                ti = self._tuples[i]
                #st = i + 1 if self._all_str_type else 0
                st = 0
                for j in range(st, len(self._tuples)):
                    if j == i:
                        continue
                    tj = self._tuples[j]
                    ev_set = list()
                    for pi in range(len(self._preds)):
                        pred = self._preds[pi]
                        if pred.op == '=':
                            if ti[pred.attr1] != tj[pred.attr2]:
                                ev_set.append(pi)
                        elif pred.op == '!=':
                            if ti[pred.attr1] == tj[pred.attr2]:
                                ev_set.append(pi)
                        elif pred.op == '<':
                            if ti[pred.attr1] >= tj[pred.attr2]:
                                ev_set.append(pi)
                        elif pred.op == '<=':
                            if ti[pred.attr1] > tj[pred.attr2]:
                                ev_set.append(pi)
                        elif pred.op == '>':
                            if ti[pred.attr1] <= tj[pred.attr2]:
                                ev_set.append(pi)
                        else:
                            if ti[pred.attr1] < tj[pred.attr2]:
                                ev_set.append(pi)
                    self._evidence_set.add(' '.join(str(v) for v in ev_set))

            logging.info('vertex: {} edge: {}'.format(
                len(self._preds), len(self._evidence_set)))

            with open(os.path.join(self._output_path, 'v{}_e{}.txt'.format(len(self._preds), len(self._evidence_set))), 'w+', encoding='utf-8') as f:
                for edge in self._evidence_set:
                    f.write(edge.decode('utf8') + '\n')

        except Exception as e:
            logging.error('encounter error: {}'.format(str(e)))
            traceback.print_exc()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--dataset', type=str, default='Hospital,Tax')
    parser.add_argument('--tuple_list', type=str,
                        default='50000')
    args = parser.parse_args()
    datasets = args.dataset.split(',')
    tuple_nums = [int(num) for num in args.tuple_list.strip().split(',')]
    for dataset in datasets:
        dir = os.path.join(os.path.split(os.path.realpath(__file__))[
                           0], 'data/{}_evidence'.format(dataset.lower()))
        if os.path.exists(dir):
            shutil.rmtree(dir)
        os.mkdir(dir)
    try:
        for dataset in datasets:
            dir = os.path.split(os.path.realpath(__file__))[0]
            original_data_path = os.path.join(
                dir, 'data/{}.csv'.format(dataset))
            with open(original_data_path, 'r', encoding='utf-8') as f:
                lines = f.readlines()
            for tuple_num in tuple_nums:
                tmp_path = os.path.join(dir, 'tmp.csv')
                with open(tmp_path, 'w+', encoding='UTF-8') as part_dataset:
                    part_dataset.write(lines[0])
                    start_index = random.randint(1, len(lines) - tuple_num)
                    resultList = lines[start_index:][:tuple_num]
                    for i in resultList:
                        part_dataset.write(i)
                generator = EvidenceSetGenerator(input_path=tmp_path,
                                                 output_path=os.path.join(dir, 'data/{}_evidence'.format(dataset.lower())))
                generator.run()
    except Exception as e:
        logging.error('encounter error: {}'.format(str(e)))
    finally:
        os.remove(tmp_path)


if __name__ == '__main__':
    main()
