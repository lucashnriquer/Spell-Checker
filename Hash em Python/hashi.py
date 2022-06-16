import time

def LoadDict(fileNamedict):
        file = open(fileNamedict,'r')
        dicti = {}
        text = file.readline() 
        while text:
                text = text.rstrip()
                if text not in dicti.keys():
                        dicti[text]=text
                text = file.readline()

        file.close()
        return dicti


def Search(testFile,dicti):
        testFile = open(testFile,"r")
        resultFile = open("conclusion.txt","w")

        linha2 = testFile.readline()
        timer1 = time.time() * 1000
        while linha2:
                text = linha2.split()
                for string in text:
                        if string not in dicti:
                                resultFile.write(string + "\n")
                
                linha2 = testFile.readline()

        timer2 = time.time() * 1000
        print(timer2-timer1)
        testFile.close()
        resultFile.close()

def main():
        fileNamedict = "dictionary.txt"
        testFile = "txt.txt"
        dic = LoadDict(fileNamedict)
        Search(testFile,dic)





main()
