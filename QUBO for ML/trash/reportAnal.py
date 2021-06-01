import sys
from pandas import DataFrame as DF
from numpy import array as AR
from numpy import mean
from plotly.subplots import make_subplots
import plotly.graph_objects as go

def max_red(col):
	maxE = max(abs(col))
	is_max = abs(abs(col) - abs(maxE)) < 10e-10
	return ['color: red' if v else 'color: black' for v in is_max]

STYLE = ''' 
<head>
  <style>
  table {
    border-collapse: collapse;
    table-layout: fixed;
  }
  th {
    height: 40px;
    border-collapse: collapse;
    width: 100px;
  }
  td {
    text-align: center;
    border-collapse: collapse;
    height: 30px;
    width: 100px;
  }
  th, td {
    border-bottom: 1px solid #ddd;
  }
  tr:hover {background-color: #f5f5f5;}
  </style>
</head>'''

A = []
B = []
Af = []
Bf = []
MSE = []

out = sys.argv[1].split('/')
for file in sys.argv[1:]:
    if ".rpt" in file:
        fp = open(file, "r")
        lines = fp.readlines()
        fp.close()
        for l in lines:
            if "A:" in l:
                A.append(float(l.split()[2]))
            if "B:" in l:
                B.append(float(l.split()[2]))
            if "Linear regression solution" in l:
                l = l.replace('[', ' ')
                l = l.replace(']', ' ')
                Af.append(float(l.split()[4]))
                Bf.append(float(l.split()[5]))
            if "Number of elements in dataset:" in l:
                N = int(l.replace("- Number of elements in dataset:", ""))
            if "Precision array:" in l:
                P = l.replace("\t- Precision array:", "")
            if "- Number of binary variables: " in l:
                Nw = int(l.replace("- Number of binary variables:", ""))
            if "- MSE: " in l:
                MSE.append(float(l.replace("- MSE: ", "")))


dA = AR(A) - AR(Af)
dB = AR(B) - AR(Bf)
meanA = mean(dA)
meanB = mean(dB)
data = DF(list(zip(A, B, Af, Bf, dA, dB, MSE)), columns=['A', 'B', 'found A', 'found B', 'delta A', 'delta B', 'MSE'])
data = data.sort_values('A')
st = data.style.apply(max_red, subset = ['delta A', 'delta B'])
html = data.to_html()
html = html.replace("border=\"1\"", "")
html = html.replace("    <tr style=\"text-align: right;\">","    <tr>")

fig = make_subplots(rows=1, cols=2, subplot_titles=("Slope delta", "Intercept delta"))
fig.add_trace(go.Bar(x=data['A'], y=data['delta A']), row=1, col=1)
fig.add_trace(go.Bar(x=data['A'], y=data['delta B']), row=1, col=2)
fig.update_layout(showlegend=False)
fig.write_html(f'{out[1]}.html')

with open(f"{out[1]}.html", "a") as of:
    of.write("<body>\n")
    of.write(f"<br><br>Dataset contains {N} points<br>")
    of.write(f"Problem has {Nw} binary variables<br>")
    of.write(f"Precision array contains {P}<br>")
    of.write(f"Mean error on A is: {meanA}<br>")
    of.write(f"Mean error on B is: {meanB}<br><br><br><br>")
    of.write("</body>\n")
    of.writelines(STYLE)
    of.writelines(st.render())
    # of.writelines(html)


