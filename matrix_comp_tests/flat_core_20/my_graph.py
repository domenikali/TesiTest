import os 
import matplotlib.pyplot as plt
import numpy as np
import re
from pathlib import Path


def import_vector(path):
    vector = []
    

    with open(path, "r") as f:
        for line in f:
            element = line.replace('\n','')
            vector.append(float(element))

        
    return str(path).replace('_mvm.txt','') ,vector
    

def better (values):
    bet={}
    for key, array in values.items():
        if key == "std":
            continue
        else:
            bet[np.mean(values["std"])/np.mean(array) ] = key

    print(np.mean(values[bet[max(bet.keys())]]))
    return bet[max(bet.keys())]+ " is " +str(round(max(bet.keys()),2)) + " times faster than std (on average)"


def plot(values):
    path =os.path.dirname(os.path.abspath(__file__))
    n_core=os.path.basename(path)

    val = []
    labels = []

    

    fig, ax = plt.subplots(figsize=(30, 6))
    for key,array in values.items():
        ax.plot(array, label=key)
    

    

    ax.legend()
    ax.set_yscale('log')
    ax.set_xlabel("Number of iterations")
    ax.set_ylabel("Time (s)")
    ax.set_title("Matrix multiplication time on "+n_core+" machinne")
    ax.set_gid(True)

    plt.savefig('my_figure.png',dpi=500)


def violin(values):
    path =os.path.dirname(os.path.abspath(__file__))
    n_core=os.path.basename(path)
    val = []
    labels = []

    for key,array in values.items():
        val.append(array)
        labels.append(key)

    fig, ax = plt.subplots()
    ax.set_title("Matrix multiplication time on "+n_core+" machine")
    ax.set_ylabel("Time (s)")
    ax.set_xlabel("Method")
    vp = ax.violinplot(val, showmeans=True, showmedians=False)
    ax.set_xticks(range(1, len(values) + 1))
    ax.set_xticklabels(labels)

    plt.annotate(better(values),
            xy = (0.5, -0.2),
            xycoords='axes fraction',
            ha='center',
            va="bottom",
            fontsize=8,
            fontstyle='italic')
    
    fig.tight_layout()
    ax.set_yscale('log')

    plt.savefig('my_violin.png',dpi=500)


def boxplot(values):
    path =os.path.dirname(os.path.abspath(__file__))
    n_core=os.path.basename(path)

    val = []
    labels = []

    for key,array in values.items():
        val.append(array)
        labels.append(key)
    
    fig, ax = plt.subplots()
    ax.set_title("Matrix multiplication time on "+n_core+" machine")
    ax.set_ylabel("Time (s)")
    ax.set_xlabel("Method")
    bp = ax.boxplot(val, patch_artist=True,tick_labels=labels,whis=5.0)

    
    ax.set_yscale('log')

    plt.savefig('my_boxplot.png',dpi=500)

def sort_key(filename):
    if filename == "std_mvm.txt":
        return (0, 0)
    
    match = re.search(r'(\d+)', filename)
    if match:
        number = int(match.group(1))
    else:
        number = float('inf')
    
    return (1, number)

if __name__ == "__main__":

    path =Path('.')
    values ={}
    paths =[]
    for txt_file in path.glob("*.txt"):
        paths.append(str(txt_file))
    
    sorted_paths = sorted(paths, key=sort_key)
    for txt_file in sorted_paths:
        key, array = import_vector(txt_file)
        values[key] = array

    boxplot(values)
    plot(values)
    violin(values)

    
    

    
