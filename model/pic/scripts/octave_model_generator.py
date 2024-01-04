#!/usr/bin/python2.7

import sys
import re

from argparse import ArgumentParser

NY = 10
TIMESTEPS = 200


def parse_options():
    parser = ArgumentParser()

    parser.add_argument('-n', '--np', dest='np', action='store',
                        type=int, nargs='+', default=1,
                        help='number of particles of each step')

    parser.add_argument('-m', '--model-path', dest='model_path', action='store',
                        type=str, required=True, help='octave model path')

    parser.add_argument('-o', '--output-path', dest='output_path', action='store',
                        type=str, default='.', help='output path')

    options = parser.parse_args()

    return options


def read_model(model_path):
    with open(model_path, 'r') as _f:
        content = _f.read()
    return content

def save_instance(n, instance, output_path):
    filename = '%s/%d.m' % (output_path, n)
    with open(filename, 'w') as _f:
        _f.write(instance)

def get_instance(content, n):
    np_regexp = 'np_idx = (\d+);'
    max_part_regexp = 'max_part=(\d+);'
    max_part = n*(NY-1)*TIMESTEPS
    content = re.sub(np_regexp, r'np_idx = %d;'%n, content)
    content = re.sub(max_part_regexp, r'max_part = %d;'%max_part, content)
    return content

def generate_models(options):
    content = read_model(options.model_path)
    for n in options.np:
        instance = get_instance(str(content), n)
        save_instance(n, instance, options.output_path)


if __name__ == '__main__':
    options = parse_options()
    generate_models(options)
