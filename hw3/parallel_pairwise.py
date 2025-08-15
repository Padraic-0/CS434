import numpy as np
import multiprocessing as mp
import os

def compute_pairwise_products(data):
    """
    Compute pairwise products for the dataset and return the resulting array.
    """
    n_samples, n_features = data.shape
    pairwise_features = np.array([
        data[:, i] * data[:, j] 
        for i in range(n_features) 
        for j in range(i, n_features)
    ]).T
    return np.hstack((data, pairwise_features))

def process_file(input_file, output_file, normalize=True):
    """
    Read the input file, compute pairwise features, and save to output file.
    """
    # Load data
    data = np.loadtxt(input_file, delimiter=",", dtype=np.float64)
    X = data[:, :-1]
    y = data[:, -1][:, np.newaxis]

    # Normalize if needed
    if normalize:
        X = X / 256 - 0.5

    # Compute pairwise products
    X_with_pairwise = compute_pairwise_products(X)

    # Combine features with labels
    final_data = np.hstack((X_with_pairwise, y))

    # Save to output file
    np.savetxt(output_file, final_data, delimiter=",")
    print(f"Processed {input_file} -> {output_file}")

def parallel_process_files(file_info, normalize=True):
    """
    Process multiple files in parallel.
    file_info: List of tuples [(input_file, output_file), ...]
    """
    # Create a pool of workers
    with mp.Pool(processes=mp.cpu_count()) as pool:
        # Map each file to the process_file function
        pool.starmap(process_file, [(infile, outfile, normalize) for infile, outfile in file_info])

if __name__ == "__main__":
    # Define input and output file paths
    file_info = [
        ("mnist_small_train.csv", "mnist_small_train_with_pairwise.csv"),
        ("mnist_small_val.csv", "mnist_small_val_with_pairwise.csv"),
        ("mnist_small_test.csv", "mnist_small_test_with_pairwise.csv"),
    ]

    # Process files in parallel
    parallel_process_files(file_info, normalize=True)