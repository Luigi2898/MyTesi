from os.path import abspath
import glob
from numpy import array
from numpy.core.fromnumeric import mean
from numpy.core.shape_base import block
from numpy.lib.function_base import append
from matplotlib import pyplot as plt

class MSEt():
    """Class to handle MSE and simulation parameters
    """

    def __init__(self, NP : int, NA : int, DA : int, P : int) -> None:
        """Initailizes the class

        Args:
            NP (int) : Number of points in dataset
            NA (int) : Number of annealings
            DA (int) : Duration of every single annealing
            P  (int) : HALF the dimension of the precision vector
        """
        self.NP = NP
        self.NA = NA
        self.DA = DA
        self.P = P
    
    def __getitem__(self, key : str):
        try:
            if key == "NP":
                return self.NP
            elif key == "NA":
                return self.NA
            elif key == "DA":
                return self.DA
            elif key == "P":
                return self.P
            elif key == "MSE":
                return self.MSE
            elif key == "MEAN":
                return self.MSEmean
            elif key == "S":
                return self.S
            elif key == "SMean":
                return self.Smean
            else:
                raise "No such a field"
        except:
          print(f"Exception occured, wrong field to MSE type. \"{key}\" is not valid")
          exit()
            
    def readMSE(self) -> None:
        """Permits to read MSE from .rpt files, it reads files for all the slopes
        """
        MSE = []
        S = []
        folder = f"../output/Regressione lineare/sim/{self.NP}P/{self.NA}Annealing/{self.DA}Duration/{self.P}Precision/"
        folder = abspath(folder)
        try:
          files = glob.glob(f"{folder}/*.rpt")
          if files == []:
                raise "No file in directory"
        except:
            print(f"No file in dir: {folder}")
            exit()  
        for f in files:
            with open(f, 'r') as inF:
                lines = inF.readlines()
                for l in lines:
                    if "- MSE:" in l:
                        rmse = l.split(" ")[2]
                    if "- A:" in l:
                        rA = l.split(" ")[2]
                    if "- Linear regression solution: [" in l:
                        l = l.replace("- Linear regression solution: [", "")
                        rAf = l.split(" ")[0]
            S.append(abs(float(rAf) - float(rA)))
            MSE.append(float(rmse))        
        self.MSE = array(MSE)
        self.S = array(S)
        self.Smean = mean(self.S)
        self.MSEmean = mean(self.MSE)
    

class result():
    """Handles an array of MSE obj and permits to plot MSE wrt any sim varible
    """
    def __init__(self) -> None:
        """Initialize class
        """
        self.MSE = []
        self.np = []
        self.na = []
        self.da = []
        self.p = []
        self.dic = {}
    
    def addMSE(self, MSE2add : MSEt) -> None:
        self.MSE.append(MSE2add)
        self.addNew(MSE2add.NP, self.np)
        self.addNew(MSE2add.NA, self.na)
        self.addNew(MSE2add.DA, self.da)
        self.addNew(MSE2add.P, self.p)
        self.dic = {'NP' : self.np, 'NA' : self.na, 'DA' : self.da, 'P' : self.p}

    def getKEY(self):
        return ["NP", "NA", "DA", "P"]  

    def MSEvsPAR(self, PAR : str, PAR2 : str, FIX1 : str, FIX2 : str, VAL1 : int, VAL2 : int) -> None:
        output = []
        param = self.dic[PAR2]
        for p in param:
            x = []
            y = []
            for anMSE in self.MSE:
                if p == anMSE[PAR2] and anMSE[FIX1] == VAL1 and anMSE[FIX2] == VAL2:
                    y.append(anMSE['SMean'])
                    x.append(anMSE[PAR])
            output.append([x, y])
        plt.figure()
        for i, _ in enumerate(param):         
            plt.plot(output[i][0], output[i][1], '*', markersize=10)
        plt.legend(self.dic[PAR2])
        plt.grid(True)
        plt.title(f"Slope error mean VS {PAR} wrt {PAR2}\n{FIX1} = {VAL1} - {FIX2} = {VAL2}")
        plt.show(block=False)
        plt.savefig(f"{PAR}_wrt_{PAR2}_{FIX1}_{VAL1}__{FIX2}_{VAL2}.png")


    def addNew(self, el : any, l : list) -> None:
        """Checks if el is in l, if not adds it to l

        Args:
            el (any): element to be added
            l (list): list to receive el
        """
        if not(el in l):
          l.append(el)

if __name__ == "__main__":

    res = result()
    NP = [100, 1000, 10000]
    P  = [2, 3, 4, 5, 10, 15, 20]
    NA = [1, 100, 1000]
    DA = [1000, 10000, 100000]
    for np in NP:
        for p in P:
            for na in NA:
                for da in DA:
                    newMSE = MSEt(np, na, da, p)
                    newMSE.readMSE()
                    res.addMSE(newMSE)
    
    for n in NA:
        for p in DA:
            res.MSEvsPAR('P', 'NP', 'NA', 'DA', n, p)

    res.MSEvsPAR('NA', 'DA', 'P', 'NP', 15, 1000)
    res.MSEvsPAR('NA', 'DA', 'P', 'NP', 10, 1000)
    input()