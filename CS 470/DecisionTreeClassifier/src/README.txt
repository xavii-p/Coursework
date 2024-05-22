To run the code in DecisionTree.py, make sure the following requirements are met:

ENVIRONMENTS:
-- Python 3.12.2

LIBRARIES: (each library below should be a version that is compatible with Python 3.12.2)
-- Pandas Version 1.23
-- NumPy Version 1.5

EXECUTION:

    To run the script, navigate to the directory containing the script and use the following syntax:
    python DecisionTree.py <data_file_path> <training_set_file_path> <test_set_file_path> <test_label_output_file_path>

        If you are using the relative file path, the CWD (current working directory) should be where the files are stored.

        -- <data_file_path>: This should be the relative or absolute file path to the dataset, ending in .csv. This file must contain the dataset to be used for training and testing the decision tree model.

        -- <training_set_file_path>: This file should contain a list of IDs that correspond to the rows in the dataset that will be used as the training set.

        -- <test_set_file_path>: This file should list the IDs that correspond to the rows in the dataset that will be used as the test set.

        -- <test_label_output_file_path>: This is the file path where the predictions of the decision tree model will be saved. Ensure this path ends in .txt for compatibility with the script's output formatting.

DESCRIPTION:

    The DecisionTree.py implements a basic decision tree classifier from scratch. It uses the Gini index to determine the best splits and recursively builds a binary tree for classification tasks. The script handles categorical and continuous data, processes training and test data based on provided IDs, trains the decision tree model, and outputs predictions.

MODIFYING THE SCRIPT FOR DIFFERENT DATASETS:

    If you are using a dataset different from the one originally intended for use with this script, you must ensure that categorical attributes (if any) that you would like to one-hot encode (create a new column for each value this attribute can take and place 0 or 1 e.x. Gender (M,F) --> Gender_M and Gender_F) are properly encoded as dummy variables.

    This can be done by replacing the caterogical attributes from the original dataset with the ones relevant to your dataset in the main method or otherwise leaving it empty.

    The lines of code like:
    train_data = data[data['person ID'].isin(train_ids)]
    or:
    trainFeatures = train_data.drop(['person ID', 'Has heart disease?'], axis=1).values
    need to be modified to address the appropriate attribute names in your dataset. Replace `'person ID'` with the identifier column in your dataset, and `'Has heart disease?'` with your target variable. Ensure all column references match those in your dataset to avoid runtime errors.

EXAMPLE USAGE:

    python DecisionTree.py data.csv para2_file.txt para3_file.txt para4_file.txt
