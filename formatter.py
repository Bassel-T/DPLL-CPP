# Take in the sudoku output and print it in a pretty way

with open("output.txt", "r") as f:
    line = f.readline()

    # Read until completion
    while not line.startswith("Completed"):
        line = f.readline()

    line = f.readline()
    a = []

    # Generate an empty board
    for i in range(10):
        a.append([])
        for j in range(10):
            a[i].append(0)

    # Translate symbols into board positions
    while not line == "":
        if not line.startswith("-"):
            a[ord(line[0]) - 65][ord(line[1]) - 65] = line[2:3]

        line = f.readline()
            
        
    # Display board
    for i in range(9):
        for j in range(9):
            print(str(a[i][j]) + " ", end='')
        
        print("\n")