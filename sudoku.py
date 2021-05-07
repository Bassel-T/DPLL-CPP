# AA1 AB2 AC3
# [Row][Column][Value]
tileName = ["A", "B", "C" ,"D" ,"E", "F", "G", "H", "I"]

regions = [
    ["AA", "AB", "AC", "BA", "BB", "BC", "CA", "CB", "CC"],
    ["DA", "DB", "DC", "EA", "EB", "EC", "FA", "FB", "FC"],
    ["GA", "GB", "GC", "HA", "HB", "HC", "IA", "IB", "IC"],
    ["AD", "AE", "AF", "BD", "BE", "BF", "CD", "CE", "CF"],
    ["DD", "DE", "DF", "ED", "EE", "EF", "FD", "FE", "FF"],
    ["GD", "GE", "GF", "HD", "HE", "HF", "ID", "IE", "IF"],
    ["AG", "AH", "AI", "BG", "BH", "BI", "CG", "CH", "CI"],
    ["DG", "DH", "DI", "EG", "EH", "EI", "FG", "FH", "FI"],
    ["GG", "GH", "GI", "HG", "HH", "HI", "IG", "IH", "II"]
]

def min(a, b):
    if a < b:
        return a
    
    return b

with open("sudoku.cnf", "w") as f:
    # Each slot must have at least one
    for i in tileName:
        for j in tileName:
            for k in range(1, 9):
                f.write(i + j + str(k) + " ")
            
            f.write(i + j + "9\n")

    for i in tileName:
        for j in tileName:
            for k in range(1, 10):
                for m in range(k + 1, 10):
                    f.write("-" + i + j + str(k) + " -" + i + j + str(m) + "\n")

    for i in range(len(tileName)):
        for j in range(len(tileName)):
            for k in range(min(i, j), len(tileName)):
                for m in range(1, 10):
                    if k > j:
                        f.write("-" + tileName[i] + tileName[j] + str(m) + " -" + tileName[i] + tileName[k] + str(m) + "\n")

                    if k > i:
                        f.write("-" + tileName[i] + tileName[j] + str(m) + " -" + tileName[k] + tileName[j] + str(m) + "\n")

    for box in regions:
        for i in range(len(box)):
            for j in range(i + 1, len(box)):
                for k in range(1, 10):
                    f.write("-" + box[i] + str(k) + " -" + box[j] + str(k) + "\n")