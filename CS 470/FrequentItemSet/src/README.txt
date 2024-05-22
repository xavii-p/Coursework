---- HONOR CODE STATEMENT ----

THIS  CODE  IS  MY  OWN  WORK,  IT  WAS  WRITTEN 
WITHOUT  CONSULTING  CODE  WRITTEN  BY  OTHER  STUDENTS.  Xavier_Pierce


To run the code in FrequentItemset.py, you will need the following:

ENVIRONMENTS:
-- Python 2.7.16

LIBRARIES:  (each library below should be a version that is compatible with Python 2.7.16)
-- Pandas Version 0.24.2


To run the script, navigate to the directory containing the script and use the following syntax:
    python FrequentItemset.py <input_file_path> <min_support> <output.txt>

    -- The parameter input_file_path should be the relative or absolute file path to the data file, 
    ending in .csv

    -- The parameter min_support is the support value you can tune to find itemsets with a support count 
    meeting this minimum

    -- The parameter output.txt is the output file path that the frequent itemsets will be written to; the 
    file name should be named "output.txt" for the script to run correctly. For instance, the path
    itemsets/output/output.txt would work, however itemsets/output/output1.txt would not.

EXAMPLE USAGE:
python DecisionTree.py data.csv para2_file.txt para3_file.txt para4_file.txt