
''' 
---- COLLABORATION STATEMENT ----
No collaboration existed for the completion of this project
'''

import pandas as pd
from itertools import combinations
from collections import defaultdict

class FrequentItemset:
        
    def __init__(self, input_file, min_support, output_file):
        self.input_file = input_file
        self.min_support = min_support
        self.output_file = output_file
    
    def read_transactions(self):
        data = pd.read_csv(self.input_file)
        transactions = data['text_keywords'].str.split(';').tolist()
        # Convert each transaction to a set for easier processing
        transactions = [set(transaction) for transaction in transactions]
        return transactions
    
    def apriori_algorithm(self, transactions):
        def generate_candidates(prev_itemsets, k):
            candidates = set()
            for itemset1 in prev_itemsets:
                for itemset2 in prev_itemsets:
                    # Ensure you're comparing frozensets for consistency
                    union_set = itemset1.union(itemset2)
                    if len(union_set) == k:
                        candidates.add(union_set)
            return candidates

        item_count = defaultdict(int)
        # Start by counting support for 1-itemsets
        for transaction in transactions:
            for item in transaction:
                item_count[frozenset([item])] += 1

        # Filter out itemsets that don't meet min_support
        freq_itemsets = {itemset: count for itemset, count in item_count.items() if count >= self.min_support}
        all_freq_itemsets = freq_itemsets.copy()

        k = 2
        while True:
            prev_itemsets = set(freq_itemsets.keys())
            candidates = generate_candidates(prev_itemsets, k)
            candidate_count = defaultdict(int)
            for candidate in candidates:
                for transaction in transactions:
                    if candidate.issubset(transaction):
                        candidate_count[candidate] += 1

            freq_itemsets = {itemset: count for itemset, count in candidate_count.items() if count >= self.min_support}
            if not freq_itemsets:
                break

            all_freq_itemsets.update(freq_itemsets)
            k += 1

        # Prepare the final list of frequent itemsets with their counts
        all_freq_itemsets_with_count = [(list(itemset), count) for itemset, count in all_freq_itemsets.items()]
        all_freq_itemsets_sorted = sorted(all_freq_itemsets_with_count, key=lambda x: x[1], reverse=True)

        return all_freq_itemsets_sorted

    def write_output(self, frequent_itemsets):
        with open(self.output_file, 'w') as output:
            for itemset, count in frequent_itemsets:
                # Ensure itemset is converted to a sorted list for consistent output
                formatted_itemset = '{} ({})'.format(' '.join(sorted(itemset)), count)
                output.write(formatted_itemset + '\n')

    def run(self):
        transactions = self.read_transactions()
        frequent_itemsets = self.apriori_algorithm(transactions)
        self.write_output(frequent_itemsets)
        print('Frequent itemsets have been written to {}'.format(self.output_file))

## main method, takes 3 parameters when running in shell
if __name__ == "__main__":
    import sys
    if len(sys.argv) != 4 or sys.argv[3] != "output.txt":
        print('Usage: python script.py <input_file> <min_support> <output.txt>')
        sys.exit(1)

    print(pd.__version__)
    input_file, min_support, output_file = sys.argv[1], int(sys.argv[2]), "output.txt"
    apriori = FrequentItemset(input_file, min_support, output_file)
    apriori.run()