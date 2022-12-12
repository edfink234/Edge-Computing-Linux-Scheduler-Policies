#Policy options:
# -b, --batch          set policy to SCHED_BATCH
# -d, --deadline       set policy to SCHED_DEADLINE
# -f, --fifo           set policy to SCHED_FIFO
# -i, --idle           set policy to SCHED_IDLE
# -o, --other          set policy to SCHED_OTHER
# -r, --rr             set policy to SCHED_RR (default)

import subprocess
from time import sleep
import os
import pandas as pd
policies = ('--batch','--fifo','--idle','--other','--rr')
min_priorities = (0,1,0,0,1)

data = pd.DataFrame()

for (policy,min_priority) in zip(policies,min_priorities):
    sub_data = pd.DataFrame()
    for i in range(10):
        proc = subprocess.Popen(["./LinuxScheduler"], stdout=subprocess.PIPE, shell=True)
        pid = os.popen("sudo pidof -s ./LinuxScheduler").read()

        os.system(f"sudo chrt {policy} -p {min_priority} {pid}")
        os.system(f"chrt -p {pid}")
        
        proc.communicate()
        if sub_data.empty:
            sub_data = pd.read_csv("ThreadTimes.txt",header=None,index_col=0).T
        else:
            temp = pd.read_csv("ThreadTimes.txt",header=None,index_col=0).T
            sub_data = pd.concat((sub_data,temp),sort=False)
            
    old_cols = sub_data.columns
    new_cols = [f"{i} {policy}" for i in old_cols]
    cols_dict = {key:val for (key,val) in zip(old_cols,new_cols)}
    sub_data.rename(columns=cols_dict,inplace=True)
    if data.empty:
        data = sub_data
    else:
        data=pd.concat((data,sub_data), axis = 1)
   
data.to_csv("LinuxSchedulerTimesAllThreads.txt")
