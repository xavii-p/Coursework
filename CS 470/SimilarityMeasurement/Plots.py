import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd

# Assuming 'data' is your DataFrame
file = '/Users/xavierpierce/My Stuff/School/Spring 2024/CS 470/HW1/data.csv'
data = pd.read_csv(file)

sns.set()  # Use Seaborn styles

# Generate 3 box plots for different attributes
'''plt.figure(figsize=(12, 6))
sns.boxplot(x=data['age'])
plt.title('Box Plot of Age')
plt.show()

plt.figure(figsize=(12, 6))
sns.boxplot(x=data['resting blood pressure'])
plt.title('Box Plot of Resting Blood Pressure')
plt.show()

plt.figure(figsize=(12, 6))
sns.boxplot(x=data['serum cholesterol in mg/dl'])
plt.title('Box Plot of Serum Cholesterol in mg/dl')
plt.show()
'''
'''
# Generate 3 histograms for different attributes
plt.figure(figsize=(12, 6))
sns.distplot(data['age'], kde=False, bins=20)  # kde=False turns off the density curve
plt.title('Histogram of Age')
plt.ylabel('Frequency')
plt.xlabel('age')
plt.show()


plt.figure(figsize=(12, 6))
sns.distplot(data['maximum heart rate achieved'], kde=False, bins=10)  # kde=False turns off the density curve
plt.title('Histogram of Maximum Heart Rate Achieved')
plt.ylabel('Frequency')
plt.xlabel('maximum heart rate achieved')
plt.show()

plt.figure(figsize=(12, 6))
sns.distplot(data['thal: 3 = normal; 6 = fixed defect; 7 = reversable defect'], kde=False)  # kde=False turns off the density curve
plt.title('Histogram of Thal')
plt.ylabel('Frequency')
plt.xlabel('thal: 3 = normal; 6 = fixed defect; 7 = reversable defect')
plt.show()
'''
'''
# Generate 3 scatter plots for pairs of attributes
plt.figure(figsize=(12, 6))
sns.scatterplot(x='age', y='maximum heart rate achieved', data=data)
plt.title('Scatter Plot between Age and Maximum Heart Rate Achieved')
plt.show()

plt.figure(figsize=(12, 6))
sns.scatterplot(x='serum cholesterol in mg/dl', y='resting blood pressure', data=data)
plt.title('Scatter Plot between Serum Cholesterol in mg/dl and Resting Blood Pressure')
plt.show()

plt.figure(figsize=(12, 6))
sns.scatterplot(x='age', y='resting blood pressure', data=data)
plt.title('Scatter Plot between Age and Resting Blood Pressure')
plt.show() 
'''

