from itertools import combinations
import pandas as pd
import numpy as np


def calculate_numerical_dissimilarity(val1, val2):
    """Calculate dissimilarity for numerical attributes."""
    return np.abs(val1 - val2)

def calculate_nominal_dissimilarity(val1, val2):
    """Calculate dissimilarity for nominal attributes (simple matching)."""
    return 0 if val1 == val2 else 1

file = '/Users/xavierpierce/My Stuff/School/Spring 2024/CS 470/HW1/data.csv'
data = pd.read_csv(file)

attributes = {
    'numerical': ['age', 'resting blood pressure', 'serum cholesterol in mg/dl', 'maximum heart rate achieved', 'oldpeak = ST depression induced by exercise relative to rest', 'number of major vessels (0-3) colored by flourosopy'],
    'nominal': ['gender', 'chest pain type', 'fasting blood sugar > 120 mg/dl', 'resting electrocardiographic results', 'exercise induced angina', 'the slope of the peak exercise ST segment', 'thal: 3 = normal; 6 = fixed defect; 7 = reversable defect', 'Has heart disease?']  # Update this with your dataset's attributes
}

instance_pairs = list(combinations(data.index, 2))

data_for_df = []  # Calculations as described above

#Calculate mean, std. deviation, & 5-number summary
for column in data.columns:
    if data[column].dtype in ['int64', 'float64']:  # Check if the column is numerical
        print('Statistics for:', column)
        print('Mean:', data[column].mean())
        print('Standard Deviation:', data[column].std())
        print('Minimum:', data[column].min())
        print('25th Percentile:', data[column].quantile(0.25))
        print('Median:', data[column].median())
        print('75th Percentile:', data[column].quantile(0.75))
        print('Maximum:', data[column].max())
        print('\n')  # Add a newline for readability

# Calculate dissimilarity for each pair and each attribute
for pair in instance_pairs:
    row = [pair[0], pair[1]]
    
    # Handle numerical attributes
    for attr in attributes['numerical']:
        dissimilarity = calculate_numerical_dissimilarity(data.at[pair[0], attr], data.at[pair[1], attr])
        row.append(dissimilarity)
    
    # Handle nominal attributes
    for attr in attributes['nominal']:
        dissimilarity = calculate_nominal_dissimilarity(data.at[pair[0], attr], data.at[pair[1], attr])
        row.append(dissimilarity)
    
    data_for_df.append(row)

# Updated column names to include both numerical and nominal attributes
column_names = ['Patient1_ID', 'Patient2_ID'] + attributes['numerical'] + attributes['nominal']

dissimilarity_df = pd.DataFrame(data_for_df, columns=column_names)

output_file_path = '/Users/xavierpierce/My Stuff/School/Spring 2024/CS 470/HW1/Step1.csv'
dissimilarity_df.to_csv(output_file_path, index=False)

print('Dissimilarity data exported successfully to {output_file_path}.')

data_for_step2 = []

for row in data_for_df:
 patient1_id, patient2_id = row[0], row[1]
 total_dissimilarity = sum(row[2:])
 data_for_step2.append([patient1_id, patient2_id, total_dissimilarity])



column_names = ['Patient1_ID', 'Patient2_ID', 'Overall Dissimilarity']
total_dissimilarity_df = pd.DataFrame(data_for_step2, columns=column_names)
step2_output = '/Users/xavierpierce/My Stuff/School/Spring 2024/CS 470/HW1/Step2.csv'
total_dissimilarity_df.to_csv(step2_output, index=False)

print('Total Dissimilarity data exported successfully to {output_file_path}.')


# Find the index of the pair with the smallest total dissimilarity
idx_smallest = total_dissimilarity_df['Overall Dissimilarity'].idxmin()
# Extract the row corresponding to the smallest dissimilarity
pair_smallest = total_dissimilarity_df.loc[idx_smallest]

# Find the index of the pair with the largest total dissimilarity
idx_largest = total_dissimilarity_df['Overall Dissimilarity'].idxmax()
# Extract the row corresponding to the largest dissimilarity
pair_largest = total_dissimilarity_df.loc[idx_largest]

# Print the results
print 'Pair with the smallest total dissimilarity: Patient1_ID=%d, Patient2_ID=%d, Total Dissimilarity=%f' % (pair_smallest['Patient1_ID'], pair_smallest['Patient2_ID'], pair_smallest['Overall Dissimilarity'])
print 'Pair with the largest total dissimilarity: Patient1_ID=%d, Patient2_ID=%d, Total Dissimilarity=%f' % (pair_largest['Patient1_ID'], pair_largest['Patient2_ID'], pair_largest['Overall Dissimilarity'])
