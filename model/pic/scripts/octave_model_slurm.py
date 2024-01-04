import sys
import glob

from argparse import ArgumentParser

NY = 10
TIMESTEPS = 200


def parse_options():
    parser = ArgumentParser()

    parser.add_argument('-d', '--duration', dest='duration', action='store',
                        type=int, default=1,
                        help='expected duration of slurm jobs (in minutes)')

    parser.add_argument('-r', '--runs', dest='runs', action='store',
                        type=int, default=1,
                        help='runs')

    parser.add_argument('-m', '--model-directory', dest='model_dir', action='store',
                        type=str, default='.', help='octave models directory')

    parser.add_argument('-o', '--output-path', dest='output_path', action='store',
                        type=str, default='.', help='output path')

    options = parser.parse_args()

    return options


def generate_jobs(options):
    job_id = 1

    for filename in glob.glob('%s/*.m' % options.model_dir):
        path = '%s/%s' % (options.model_dir, filename)
        save_job(filename, job_id, options)
        job_id += 1

def save_job(filename, job_id, options):
    job = '#!/bin/bash\n'
    job += '#SBATCH -n %d\n' % options.runs
    job += '#SBATCH -c 1\n'
    job += '#SBATCH -t 00:0%d:00\n' % options.duration
    job += '#SBATCH -p free-rider\n\n'

    job_filename = '%s/%d.sh' % (options.output_path, job_id)

    for idx in xrange(options.runs):
        job += 'srun -N 1 -n 1 --output=%s/%d_%d.log octave %s &\n' % (options.output_path, job_id, idx+1, filename)

    with open(job_filename, 'w') as _file:
        _file.write(job)


if __name__ == '__main__':
    options = parse_options()
    generate_jobs(options)
