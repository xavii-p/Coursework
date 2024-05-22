import pandas as pd
import numpy as np
import sys

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

def k_fold_cross_validation(data, k=10):
    fold_size = len(data) // k
    accuracies = []
    precisions = []
    recalls = []
    f1_scores = []
    specificities = []

    for i in range(k):
        test_data = data.iloc[i * fold_size:(i + 1) * fold_size]
        train_data = pd.concat([data.iloc[:i * fold_size], data.iloc[(i + 1) * fold_size:]])

        trainFeatures = train_data.drop(['person ID', 'Has heart disease?'], axis=1).values
        trainLabels = train_data['Has heart disease?'].values
        testFeatures = test_data.drop(['person ID', 'Has heart disease?'], axis=1).values
        testLabels = test_data['Has heart disease?'].values

        model = DecisionTree(max_depth=5)
        model.train(trainFeatures, trainLabels)
        predictions = model.predict(testFeatures)

        # Calculate metrics for each fold
        accuracy = np.mean(np.array(testLabels) == np.array(predictions))
        tp = np.sum((np.array(predictions) == 'Yes') & (np.array(testLabels) == "Yes"))
        fp = np.sum((np.array(predictions) == 'Yes') & (np.array(testLabels) == "No"))
        fn = np.sum((np.array(predictions) == 'No') & (np.array(testLabels) == "Yes"))
        tn = np.sum((np.array(predictions) == 'No') & (np.array(testLabels) == "No"))

        precision = tp / (tp + fp) if tp + fp > 0 else 0
        recall = tp / (tp + fn) if tp + fn > 0 else 0
        f1 = 2 * (precision * recall) / (precision + recall) if (precision + recall) > 0 else 0
        specificity = tn / (tn + fp) if tn + fp > 0 else 0

        accuracies.append(accuracy)
        precisions.append(precision)
        recalls.append(recall)
        f1_scores.append(f1)
        specificities.append(specificity)

    # Print average metrics
    print(f"Average Accuracy: {np.mean(accuracies):.2f}")
    print(f"Average Precision: {np.mean(precisions):.2f}")
    print(f"Average Recall: {np.mean(recalls):.2f}")
    print(f"Average F1-Score: {np.mean(f1_scores):.2f}")
    print(f"Average Specificity: {np.mean(specificities):.2f}")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print('Usage: python DecisionTree.py <input_file>')
        sys.exit(1)

    data = pd.read_csv(sys.argv[1])
    # Include preprocessing like dummy encoding if required
    # data = pd.get_dummies(data, columns=categoricalAttributes)

    k_fold_cross_validation(data, k=10)
