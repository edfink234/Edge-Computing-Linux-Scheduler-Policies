import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import os
import warnings
warnings.filterwarnings("ignore") # MatplotlibDeprecationWarning: The resize_event function was deprecated in Matplotlib 3.6 and will be removed two minor releases later. Use callbacks.process('resize_event', ResizeEvent(...)) instead.

#os.system('scp magix@detlab-101.prisma.uni-mainz.de:"/home/magix/LinuxSchedulerTimesAllThreads.txt" ./')
hists = {}
policies = ('--batch','--fifo','--idle','--other','--rr')

data = pd.read_csv("LinuxSchedulerTimesAllThreads.txt",index_col=False)
if "Unnamed: 0" in data.columns:
    data.drop("Unnamed: 0", axis=1, inplace=True)

finalTotalList = ("total " + _ for _ in policies)
finalTotal = {key:value for (key,value) in zip(finalTotalList,[pd.Series(dtype="float64") for i in range(len(policies))])}
finalThreadList = ("thread " + _ for _ in policies)
finalThread = {key:value for (key,value) in zip(finalThreadList,[pd.Series(dtype="float64") for i in range(len(policies))])}

for col in data.columns:
    
    if col[0].isdigit():
        for policy in policies:
            if policy in col:
                temp = finalThread["thread "+policy]
                finalThread["thread "+policy] = pd.concat((data[col],temp))
    else:
        for policy in policies:
            if policy in col:
                temp = finalTotal["total "+policy]
                finalTotal["total "+policy] = pd.concat((data[col],temp))
                
for i,j in enumerate(finalThread):
    plt.scatter(i,finalThread[j].mean(), label = j)
    plt.errorbar(i,finalThread[j].mean(), yerr = finalThread[j].std())
plt.legend()
plt.xticks([],[])
plt.ylabel("Time (Nano-seconds)")
plt.title("Thread Run-Time Averages Random Number Generation")
plt.savefig("Thread_Averages_Random_Numbers.png",dpi=5*96)
plt.close()

for i,j in enumerate(finalTotal):
    plt.scatter(i,finalTotal[j].mean(), label = j)
    plt.errorbar(i,finalTotal[j].mean(), yerr = finalTotal[j].std())
plt.legend()
plt.xticks([],[])
plt.ylabel("Time (Nano-seconds)")
plt.title("Total Run-Time Averages Random Number Generation")
plt.savefig("Total_Averages_Random_Numbers.png",dpi=5*96)
plt.close()

for col in data.columns:
    if col[0] not in hists:
        hists[col[0]] = [data[col]]
    else:
        hists[col[0]].append(data[col])

for i in hists:
    for j in hists[i]:
        plt.hist(j,label = j.name.split()[1],bins=10)
    plt.xlabel("Times (nanoseconds)")
    plt.ylabel("Entries")
    plt.legend()
    if i == 't':
        plt.title("Total Program Run Times Random Number Generation")
        plt.savefig(f"RunTimesLinuxSchedulerTotal.png",dpi=5*96)
    else:
        plt.title(f"Thread {i} Run Times Random Number Generation")
        plt.savefig(f"RunTimesLinuxSchedulerThread{i}.png",dpi=5*96)
    plt.close()


