from bokeh.plotting import figure , show , output_file
from bokeh.io import output_notebook#, show


x = []
y = []
# output_notebook()
for i in range(63):
    
    trace = open("./63part/ECDSA" + str(i+1) + ".txt" , "r")
    for j in range(200000): #20000000
        x.append(j)
        y.append(float(trace.readline().strip()))    
    # HTML file
    output_file("./html_plot/ECDSA" + str(i+1) +".html")

    p = figure( sizing_mode='scale_width', height=100 )
    p.line(x , y)

    show(p)

    x.clear() # x = []
    y.clear() # y = []

'''
# Test
i = 1
output_notebook()
trace = open("./63part/ECDSA" + str(i+1) + ".txt" , "r")
for j in range(2000000):
    x.append(j)
    y.append(float(trace.readline().strip()))

# HTML file
output_file("./html_plot/ECDSA" + str(i+1) +".html")
#p = figure(title = "Trace of ECDSA" + str(i+1), x_axis_label = 'Point', y_axis_label = 'Power Consumption' )
p = figure( sizing_mode='scale_width', height=100 )
p.line(x , y)
show(p)
'''