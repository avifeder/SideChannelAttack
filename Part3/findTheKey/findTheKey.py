import math


def nPrime(n):
    """
    This function calc the r and the k for the montgomery product
    :param n: The modulo
    :return: k and r
    """
    length = math.floor(math.log(int(n), 2)) + 1
    r = int(math.pow(2, length))
    rInverse = ModInverse(r, n)
    k = (r * rInverse - 1) // n
    return r, k


def ModInverse(a, n):
    """
    This function calc the inverse modulo of number a
    :param a: The number that we want his inverse
    :param n: The modulo
    :return: the inverse modulo of number a
    """
    (t, new_t, r, new_r) = 0, 1, int(n), int(a)
    while new_r != 0:
        quotient = r // new_r
        (t, new_t) = (new_t, t - quotient * new_t)
        (r, new_r) = (new_r, r - quotient * new_r)
    if t < 0:
        t = t + n
    return t


def MontgomeryProduct(a, b, n, k, r):
    """
    This function calc the montgomery product of numbers a and b
    :param a: a number we want to mult
    :param b: a number we want to mult
    :param n: The modulo
    :param k: The k for montgomery product
    :param r: The r for montgomery product
    :return: The montgomery product of numbers a and b
    """
    t = a * b
    m = t * k % r
    u = (t + m * n) // r
    return (u - n, True) if (u >= n) else (u, False)


def rsa(m, d, n, k, r):
    """
    This function encrypt/decrypt message m by rsa protocol
    :param m: The message we want to encrypt/decrypt
    :param d: The key
    :param n: The modulo
    :param k: The k for montgomery product
    :param r: The r for montgomery product
    :return: The encrypt/decrypt of message m
    """
    m_ = (m * r) % n
    x_ = (1 * r) % n
    for i in range(0, len(d)):
        x_, _ = MontgomeryProduct(x_, x_, n, k, r)
        if d[i] == '1':
            x_, _ = MontgomeryProduct(m_, x_, n, k, r)
    x, _ = MontgomeryProduct(x_, 1, n, k, r)
    return x


def rsa_sim(m, d, n, k, r, j):
    """
        This function simulate a encrypt/decrypt message m by rsa protocol
        :param m: The message we want to encrypt/decrypt
        :param d: The key
        :param n: The modulo
        :param k: The k for montgomery product
        :param r: The r for montgomery product
        :param j: The bit we want to test if we have reduce or not
        :return: The encrypt/decrypt of message m and True/False if we have reduce or not
        """
    m_ = (m * r) % n
    x_ = (1 * r) % n
    new_d = d[:j]
    new_d += '1'
    red = False
    for i in range(0, len(new_d)):
        x_, _ = MontgomeryProduct(x_, x_, n, k, r)
        if new_d[i] == '1':
            x_, red = MontgomeryProduct(m_, x_, n, k, r)
    x, _ = MontgomeryProduct(x_, 1, n, k, r)
    return x, red


def split_messages(d, n, k, r, bit, dataList):
    """
    This function split the messages to two groups. reduce in bit(bit) or not
    :param d: The key
    :param n: The modulo
    :param k: The k for montgomery product
    :param r: The r for montgomery product
    :param bit: The bit we want to test if we have reduce or not
    :param dataList: The messages
    :return: two groups. reduce in bit(bit) or not
    """
    red = []
    nored = []
    for m in dataList:
        c, bucket = rsa_sim(m[0], d, n, k, r, bit)
        if bucket:
            red.append(m)
        else:
            nored.append(m)
    return red, nored


def calcAvg(lst):
    """
    This function calc the average of a list values
    :param lst: The list
    :return: the average of a list values
    """
    if len(lst) == 0:
        return 0
    else:
        return sum(lst) / len(lst)


def testKey(data, private_key, n, k, r):
    """
    This function check if the private_key is actually the private key
    """
    test_message1 = data[0][0]
    test_message2 = data[1][0]
    sign_message1 = rsa(test_message1, str(private_key), n, k, r)
    sign_message2 = rsa(test_message2, str(private_key), n, k, r)
    return sign_message1 == data[0][1] and sign_message2 == data[1][1]


def RSATimingAttack(n, data, ratio):
    """
    This function analysis the time that took to encrypt the messages and find the private key
    :param n: The modulo
    :param data: the messages with the time
    :param ratio: the delta
    """
    (r, k) = nPrime(n)
    private_key = '1'
    bit = 1
    finished = False
    while not finished and bit < 32:
        (red, nored) = split_messages(private_key, n, k, r, bit, data)
        red_avg = calcAvg([m[2] for m in red])
        nored_avg = calcAvg([m[2] for m in nored])
        print("Difference of averages:", abs(red_avg - nored_avg))

        if abs(red_avg - nored_avg) > ratio:
            private_key += '1'
            print("The next bit is probably 1.")
        else:
            private_key += '0'
            print("The next bit is probably 0.")
        print("The private key until now is: ", private_key)
        print()

        if testKey(data, private_key, n, k, r):
            print("We did it! The private key is: \t", private_key)
            finished = True

        bit += 1

    if not finished:
        print("can't find the private key...")


def main():
    difference = 150000
    print("The first bit is 1")
    print()
    with open('timeData.txt', 'rb') as f:
        _ = f.readline()  # Ignore first line (which is a column description)
        n, e = f.readline().decode().split(',')  # read in public key
        n = int(n)
        e = int(e)
        _ = f.readline()  # ignore third line (which is a column description)
        data = [[int(x.replace("\r\n", '')) for x in line.decode().split(',')] for line in f]  # read in signature data.

    RSATimingAttack(n, data, int(difference))


if __name__ == "__main__":
    main()
