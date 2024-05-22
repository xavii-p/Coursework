'''
---- COLLABORATION STATEMENT ----
No collaboration existed for the completion of this project

---- HONOR CODE STATEMENT ----

THIS CODE IS MY OWN WORK, IT WAS WRITTEN
WITHOUT CONSULTING CODE WRITTEN BY OTHER STUDENTS. Xavier_Pierce

---
'''

import pandas as pd
import numpy as np
import sys
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score, confusion_matrix

class DecisionTree:
    def __init__(self, max_depth=None):
        self.maxDepth = max_depth
        self.labelMapping = {"Yes": 1, "No": 0}
        self.reverseMapping = {1: "Yes", 0: "No"}

    def train(self, X, y):
        y = [self.labelMapping[label] for label in y]
        self.tree_ = self._build_tree(X, y)

    def predict(self, X):
        predictions = [self._predict(inputs, self.tree_) for inputs in X]
        return [self.reverseMapping[prediction] for prediction in predictions]
    
    def _build_tree(self, X, y, depth=0):
        y = np.asarray(y)

        num_samples = X.shape[0]

        # Check if termination conditions are met: 
        if num_samples <= 1 or len(np.unique(y)) == 1 or (self.maxDepth and depth >= self.maxDepth):
            return np.bincount(y).argmax()

        # Select the best split
        best_feat, best_value = self._best_split(X, y)

        # Grow the left and right subtrees
        if best_feat is not None:
            indices_left = X[:, best_feat] <= best_value
            X_left, y_left = X[indices_left], y[indices_left]
            X_right, y_right = X[~indices_left], y[~indices_left]
            return {
                'feature_index': best_feat,
                'threshold': best_value,
                'left': self._build_tree(X_left, y_left, depth + 1),
                'right': self._build_tree(X_right, y_right, depth + 1)
            }
        return np.bincount(y).argmax()
    
    def _best_split(self, X, y):
        n = X.shape[1]
        best_gini = 1.0
        best_feat, best_value = None, None
        for feat_index in range(n):
            thresholds = np.unique(X[:, feat_index])
            for threshold in thresholds:
                gini = self._gini_index(X[:, feat_index], y, threshold)
                if gini < best_gini:
                    best_gini = gini
                    best_feat, best_value = feat_index, threshold
        return best_feat, best_value

    def _gini_index(self, feature, labels, threshold):
        feature = np.asarray(feature)
        labels = np.asarray(labels)

        def gini(labels):
            if len(labels) == 0:
                return 0
            counts = np.unique(labels, return_counts=True)[1]
            probabilities = counts / counts.sum()
            return 1.0 - sum(probabilities ** 2)

        left_labels = labels[feature <= threshold]
        right_labels = labels[feature > threshold]
        l_size = len(left_labels)
        r_size = len(right_labels)
        total_size = l_size + r_size
        if total_size == 0:
            return 0
        weighted_gini = (l_size / total_size) * gini(left_labels) + (r_size / total_size) * gini(right_labels)
        return weighted_gini
    
    def _predict(self, inputs, tree):
        while isinstance(tree, dict):
            if inputs[tree['feature_index']] <= tree['threshold']:
                tree = tree['left']
            else:
                tree = tree['right']
        return tree

## main method called in shell
if __name__ == "__main__":

    if len(sys.argv) != 5:
        print('Usage: python DecisionTree.py <input_file> <training_file_path> <test_file_path> <prediction_output_path>')
        sys.exit(1)

    data = pd.read_csv(sys.argv[1])
    train_ids = pd.read_csv(sys.argv[2], header=None).iloc[:, 0].tolist()
    test_ids = pd.read_csv(sys.argv[3], header=None).iloc[:, 0].tolist()

    # include a new column for each distinct value a categorical variable can take
    categoricalAttributes = [
        'chest pain type', 
        'resting electrocardiographic results', 
        'the slope of the peak exercise ST segment', 
        'number of major vessels (0-3) colored by flourosopy'
    ]

    if categoricalAttributes:
        data = pd.get_dummies(data, columns=categoricalAttributes)
    
    train_data = data[data['person ID'].isin(train_ids)]
    ## print(train_data.head())
    test_data = data[data['person ID'].isin(test_ids)]
    ## print(test_data.head())

    # remove person ID and target label columns from train and test data
    trainData = train_data.drop(['person ID', 'Has heart disease?'], axis=1).values
    trainLabels = train_data['Has heart disease?'].values
    testData = test_data.drop(['person ID', 'Has heart disease?'], axis=1).values
    testLabels = test_data['Has heart disease?'].values

    # optimize max depth to get better or worse accuracy --> avoid over and underfitting
    model = DecisionTree(max_depth=10)
    model.train(trainData, trainLabels)
    predictions = model.predict(testData)


    lowercaseLabel = {"Yes": "yes", "No": "no"}
    newpredictions = [lowercaseLabel[prediction] for prediction in predictions]

    with open(sys.argv[4], 'w') as f:
        for person_id, prediction in zip(test_data['person ID'].tolist(), newpredictions):
            f.write(f"{person_id} {prediction}\n")
