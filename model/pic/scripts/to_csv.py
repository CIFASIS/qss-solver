import sys

nx = 16
ny = 10
N = nx*ny


def get_output_grid(filename, time=1e20, use_nodes=False):
    grid = list()
    values = [0]

    if use_nodes:
        j = 0
        values = [0 for _ in xrange(N)]
        for i in xrange(1,2*N+1):
            f = filename % i
            lines = open(f, 'r').readlines()
            use_node = True
            for line in lines[1:]:
                a, b = line.split('\t')
                t = float(a)
                b = float(b)		
                if b == -1:
                    use_node = False
                    break
                if t >= time:
                    break
            if use_node:
                if j < 2*ny and j%2 == 0:
                    values[j/2] = b
                elif j < 2*ny:
                    values[ny + j/2] = b
                else:
                    values[j] = b
                j += 1

        for i in xrange(1, N, ny):
            row = list()
            for j in xrange(i, i+ny):
                row.append(values[j-1]) #ff.write('%.12f' % values[j-1])
                #if j < i+ny-1:
                #    ff.write(',')
            #ff.write('\n')
            grid.append(row)
    else:
        for i in xrange(1,N+1):
            f = filename % i
            lines = open(f, 'r').readlines()
            for line in lines:
                a, b = line.split('\t')
                t = float(a)
                if t >= time:
                    break
            values.append(float(b))

        for i in xrange(1, N, ny):
            row = list()
            for j in xrange(i, i+ny):
                row.append(values[j]) #ff.write('%.12f' % values[j])
                #if j < i+ny-1:
                #    ff.write(',')
            #ff.write('\n')
            grid.append(row)

    return grid

def save_grid_to_file(filename, grid):
    with open(filename, 'w') as ff:
        for row in grid:
            for i,v in enumerate(row):
                ff.write('%.12f' % v)
                if i < len(row)-1:
                    ff.write(',')
            ff.write('\n')


if __name__ == '__main__':
    use_nodes = False
    filename = '%s[%%d].dat' % sys.argv[1].upper()
    out = '%s.txt' % sys.argv[1].lower()

    if len(sys.argv) > 2:
        try:
            time = float(sys.argv[2])
        except:
            time = 1e20
            use_nodes = sys.argv[2] == 'nodes'	
    else:
        time = 1e20

    if len(sys.argv) > 3 and sys.argv[3] == 'nodes':
        use_nodes = True

    grid = get_output_grid(filename, time, use_nodes)
    save_grid_to_file(out, grid)

