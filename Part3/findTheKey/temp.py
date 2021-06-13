with open('timeData.txt', 'rb') as f:
    _ = f.readline()  # Ignore first line (which is a column description)
    n, e = f.readline().decode().split(',')  # read in public key
    n = int(n)
    e = int(e)
    _ = f.readline()  # ignore third line (which is a column description)
    for line in f:
        try:
            data = [int(x.replace("\r\n",'')) for x in line.decode().split(',')]

        #data[2] = data[2].replace("\r\n",'')
            print(data)
        except:
            print(data)
            exit()
