import random

def func1():
    DEBUG = True
    func_num = 36
    lst=[]                          # 뽑은 a를 넣어 중복 방지해주는 리스트         
    for i in range(func_num):
        a = random.randrange(1, func_num)       
        while a in lst :              # a가 이미 뽑은 리스트에 있을 때까지 다시 뽑자
            a = random.randint(1,func_num)
        lst.append(a) # 새로운 a 값을 리스트에 추가
    
    func_table = ["func"+str(i) for i in lst]
    if DEBUG: print(func_table)
    print("="*36)

    func_idx = []
    for i in range(0, func_num):
        for idx, name in enumerate(func_table):
            if str(i+1) == name[4:]:
                func_idx.append(idx)
    if DEBUG: print(func_idx)
    print("="*36)

    for idx, name in enumerate(func_table):
        rn = random.randint(1, 0xffffff)
        func_table[idx] = "&"+name+"-"+hex(rn)

    if DEBUG: print(func_table)
    print("="*36)

    for idx, val in enumerate(func_idx):
        func_idx[idx] = [val, func_table[val].split('-')[1]]

    if DEBUG: print(func_idx)
    print("="*36)


    for v in func_idx:
        print(v[1], end=', ')
    print("="*36)

    for v in func_table:
        print(v, end=', ')

# fmt = """void func{0}(unsigned long long (*arr)[6]) """

# for i in range(1, 37):
#     print(fmt.format(i)+"{}")

# def func2():
#     string = "&func1-0x7a6a77, &func6-0xfd1499, &func26-0x35469f, &func33-0x832131, &func17-0xc11741, &func19-0x46a7af, &func10-0xb62cf9, &func28-0x77a9d8, &func23-0xa4e61a, &func16-0xc2e591, &func31-0xb63c55, &func7-0xaf1834, &func12-0x6c6cb9, &func21-0x4f6498, &func29-0xb09bb0, &func25-0xec88e7, &func2-0x3f2ba2, &func27-0x4ac91c, &func20-0x4d38c0, &func32-0xcd9ca6, &func30-0x233abd, &func3-0x620c09, &func4-0xb99a98, &func22-0xd967b9, &func34-0xeaa019, &func13-0x3cc2ad, &func18-0x60c58f, &func36-0x997fc6, &func11-0x4c5f1a, &func15-0xc7ff22, &func5-0x519738, &func24-0xacecc9, &func8-0x99b07, &func14-0x246d62, &func9-0x48081b, &func35-0x763968"
#     l = string.split(', ')
#     res = []
#     for i in l:
#         i.split('-')[0]
# func2()
arr = list(range(256))
random.shuffle(arr)
print(arr)