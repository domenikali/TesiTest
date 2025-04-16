import os 
import matplotlib.pyplot as plt
import numpy as np

def import_vector(path):
    vector = []
    

    with open(path, "r") as f:
        for line in f:
            element = line.replace('\n','')
            vector.append(float(element))

        
    return vector
    
def better(std, mtd_4,mtd_8,mtd_16,mtd_20,mtd_32,mtd_512):
    labels = ['std', 'mtd_4', 'mtd_8', 'mtd_16','mtd_20','mtd_32','mtd_512']
    min_val =min(np.median(std),np.median(mtd_4),np.median(mtd_8),np.median(mtd_16),np.median(mtd_20),np.median(mtd_32),np.median(mtd_512))
    min_index = np.argmin([np.median(std),np.median(mtd_4),np.median(mtd_8),np.median(mtd_16),np.median(mtd_20),np.median(mtd_32),np.median(mtd_512)])
    return labels[min_index]+" "+str(round(np.median(std)/min_val,2))+ " times better than std"

def plot(std, mtd_4,mtd_8,mtd_16,mtd_20,mtd_32,mtd_512):
    path =os.path.dirname(os.path.abspath(__file__))
    n_core=os.path.basename(path)
    fig, ax = plt.subplots(figsize=(30, 6))

    ax.plot(std, label="std")
    ax.plot(mtd_4, label="mtd_4")
    ax.plot(mtd_8, label="mtd_8")
    ax.plot(mtd_16, label="mtd_16")
    ax.plot(mtd_20, label="mtd_20")
    ax.plot(mtd_32, label="mtd_32")
    ax.plot(mtd_512, label="mtd_512")

    

    ax.legend()
    ax.set_yscale('log')
    ax.set_xlabel("Number of iterations")
    ax.set_ylabel("Time (s)")
    ax.set_title("Matrix multiplication time on "+n_core+" machinne")
    ax.set_gid(True)

    plt.savefig('my_figure.png',dpi=500)


def violin(std, mtd_4,mtd_8,mtd_16,mtd_20,mtd_32,mtd_512):
    path =os.path.dirname(os.path.abspath(__file__))
    n_core=os.path.basename(path)
    values =[std,mtd_4,mtd_8,mtd_16,mtd_20,mtd_32,mtd_512]
    labels = ['std', 'mtd_4', 'mtd_8', 'mtd_16','mtd_20','mtd_32','mtd_512']

    fig, ax = plt.subplots()
    ax.set_title("Matrix multiplication time on "+n_core+" machine")
    ax.set_ylabel("Time (s)")
    ax.set_xlabel("Method")
    vp = ax.violinplot(values, showmeans=False, showmedians=True)
    ax.set_xticks(range(1, len(values) + 1))
    ax.set_xticklabels(labels)

    plt.annotate(better(std, mtd_4,mtd_8,mtd_16,mtd_20,mtd_32,mtd_512),
            xy = (0.5, -0.2),
            xycoords='axes fraction',
            ha='center',
            va="bottom",
            fontsize=8,
            fontstyle='italic')

    fig.tight_layout()
    ax.set_yscale('log')

    plt.savefig('my_violin.png',dpi=500)


def boxplot(std, mtd_4,mtd_8,mtd_16,mtd_20,mtd_32,mtd_512):
    path =os.path.dirname(os.path.abspath(__file__))
    n_core=os.path.basename(path)
    values =[std,mtd_4,mtd_8,mtd_16,mtd_20,mtd_32,mtd_512]
    labels = ['std', 'mtd_4', 'mtd_8', 'mtd_16','mtd_20','mtd_32','mtd_512']

    fig, ax = plt.subplots()
    ax.set_title("Matrix multiplication time on "+n_core+" machine")
    ax.set_ylabel("Time (s)")
    ax.set_xlabel("Method")
    bp = ax.boxplot(values, patch_artist=True,tick_labels=labels,whis=5.0)

    
    ax.set_yscale('log')

    plt.savefig('my_boxplot.png',dpi=500)


if __name__ == "__main__":
    std = "std_mvm.txt"
    std_veor = import_vector(std)

    mtd_4 = "mtd_4_mvm.txt"
    mtd_4_vector = import_vector(mtd_4)

    mtd_8 = "mtd_8_mvm.txt"
    mtd_8_vector = import_vector(mtd_8)

    mtd_16 = "mtd_16_mvm.txt"
    mtd_16_vector = import_vector(mtd_16)

    mtd_20 = "mtd_20_mvm.txt"
    mtd_20_vector = import_vector(mtd_20)

    mtd_32 = "mtd_32_mvm.txt"
    mtd_32_vector = import_vector(mtd_32)

    mtd_512 = "mtd_512_mvm.txt"
    mtd_512_vector = import_vector(mtd_512)
    
    std = np.array(std_veor)
    mtd_4 = np.array(mtd_4_vector)
    mtd_8 = np.array(mtd_8_vector)
    mtd_16 = np.array(mtd_16_vector)
    mtd_20 = np.array(mtd_20_vector) 
    mtd_32 = np.array(mtd_32_vector)
    mtd_512 = np.array(mtd_512_vector)

    plot(std, mtd_4,mtd_8,mtd_16,mtd_20,mtd_32,mtd_512)
    boxplot(std, mtd_4,mtd_8,mtd_16,mtd_20,mtd_32,mtd_512)
    violin(std, mtd_4,mtd_8,mtd_16,mtd_20,mtd_32,mtd_512)
    times =better(std, mtd_4,mtd_8,mtd_16,mtd_20,mtd_32,mtd_512)
    print(times)
    

    
