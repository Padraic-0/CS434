import numpy as np
import time
import math
import heapq

#constants
SIGMA = 5
NORMAL_CONSTANT = 1 / (SIGMA * math.sqrt(2 * np.pi))

def main():

    #############################################################
    # These first bits are just to help you develop your code
    # and have expected ouputs given. All asserts should pass.
    ############################################################

    # I made up some random 3-dimensional data and some labels for us
    example_train_x = np.array([ [ 1, 0, 2], [3, -2, 4], [5, -2, 4],
                                 [ 4, 2, 1.5], [3.2, np.pi, 2], [-5, 0, 1]])
    example_train_y = np.array([[0], [1], [1], [1], [0], [1]])
  
    #########
    # Sanity Check 1: If I query with examples from the training set 
    # and k=1, each point should be its own nearest neighbor
    
    for i in range(len(example_train_x)):
        assert([i] == get_nearest_neighbors(example_train_x, example_train_x[i], 1))
        
    #########
    # Sanity Check 2: See if neighbors are right for some examples (ignoring order)
    nn_idx = get_nearest_neighbors(example_train_x, np.array( [ 1, 4, 2] ), 2)
    assert(set(nn_idx).difference(set([4,3]))==set())

    nn_idx = get_nearest_neighbors(example_train_x, np.array( [ 1, -4, 2] ), 3)
    assert(set(nn_idx).difference(set([1,0,2]))==set())

    nn_idx = get_nearest_neighbors(example_train_x, np.array( [ 10, 40, 20] ), 5)
    assert(set(nn_idx).difference(set([4, 3, 0, 2, 1]))==set())

    #########
    # Sanity Check 3: Neighbors for increasing k should be subsets
    query = np.array( [ 10, 40, 20] )
    p_nn_idx = get_nearest_neighbors(example_train_x, query, 1)
    for k in range(2,7):
      nn_idx = get_nearest_neighbors(example_train_x, query, k)
      assert(set(p_nn_idx).issubset(nn_idx))
      p_nn_idx = nn_idx
   
    #########
    # Test out our prediction code
    queries = np.array( [[ 10, 40, 20], [-2, 0, 5], [0,0,0]] )
    pred = predict(example_train_x, example_train_y, queries, 3)
    #assert( np.all(pred == np.array([[0],[1],[0]])))

    #########
    # Test our our accuracy code
    true_y = np.array([[0],[1],[2],[1],[1],[0]])
    pred_y = np.array([[5],[1],[0],[0],[1],[0]])                    
    #assert( compute_accuracy(true_y, pred_y) == 3/6)

    pred_y = np.array([[5],[1],[2],[0],[1],[0]])                    
    #assert( compute_accuracy(true_y, pred_y) == 4/6)

    #######################################
    # Now on to the real data!
    #######################################
    
    # Load training and test data as numpy matrices 
    train_X, train_y, test_X = load_data()


    #######################################
    # Q9 Hyperparmeter Search
    #######################################

    # Search over possible settings of k
    print("Performing 4-fold cross validation")
    for k in [1,3,5,7,9,99,999,8000]:
        t0 = time.time()

      #######################################
      # Compute train accuracy using whole set
      #######################################
        
        correct = 0

        for index, query in enumerate(train_X):

            predicted_label = knn_classify_point(train_X, train_y, query, k)
    
            if predicted_label == train_y[index][0]:

                correct += 1


        train_acc = correct / len(train_X)
    
    

      #######################################
      # Compute 4-fold cross validation accuracy
      #######################################
        val_acc, val_acc_var = cross_validation(train_X, train_y, 4, k)
      
        t1 = time.time()
        print("k = {:5d} -- train acc = {:.2f}%  val acc = {:.2f}% ({:.4f})\t\t[exe_time = {:.2f}]".format(k, train_acc*100, val_acc*100, val_acc_var*100, t1-t0))
    
    #######################################


    #######################################
    # Q10 Kaggle Submission
    #######################################


    # set your best k value and then run on the test set
    best_k = 7

    # Make predictions on test set
    pred_test_y = predict(train_X, train_y, test_X, best_k)    
    
    # add index and header then save to file
    test_out = np.concatenate((np.expand_dims(np.array(range(2000),dtype=int), axis=1), pred_test_y), axis=1)
    header = np.array([["id", "income"]])
    test_out = np.concatenate((header, test_out))
    np.savetxt('test_predicted.csv', test_out, fmt='%s', delimiter=',')

######################################################################
# Q7 get_nearest_neighbors 
######################################################################
# Finds and returns the index of the k examples nearest to
# the query point. Here, nearest is defined as having the 
# lowest Euclidean distance. This function does the bulk of the
# computation in kNN. As described in the homework, you'll want
# to use efficient computation to get this done. Check out 
# the documentaiton for np.linalg.norm (with axis=1) and broadcasting
# in numpy. 
#
# Input: 
#   example_set --  a n-by-d matrix of examples where each row
#                   corresponds to a single d-dimensional example
#
#   query --    a 1-by-d vector representing a single example
#
#   k --        the number of neighbors to return
#
# Output:
#   idx_of_nearest --   a k-by- list of indices for the nearest k
#                       neighbors of the query point
######################################################################

def get_nearest_neighbors(example_set, query, k):
    
    l2_norm = np.linalg.norm(example_set - query, axis = 1)                 #find distance from every other point to the query

    
    sorted_neighbors = sorted(enumerate(l2_norm), key=lambda x: x[1])[:k]   #enumerate list to maintain index, sort based on the distance value
    
    idx_of_nearest = []

    for i in sorted_neighbors:
        idx_of_nearest.append(i[0])                                         #write out the index, ignoring the distance

    
    return idx_of_nearest

## I need the function to return the distance aswell for my weighting function

def get_nearest_neighbors_with_norm(example_set, query, k):
    
    l2_norm = np.linalg.norm(example_set - query, axis = 1)
    
    return heapq.nsmallest(k, enumerate(l2_norm), key=lambda x: x[1])       #sort was too slow, so heapq was used to maintain a heap of k elements, sorted based on distance


######################################################################
# Q7 knn_classify_point 
######################################################################
# Runs a kNN classifier on the query point
#
# Input: 
#   examples_X --  a n-by-d matrix of examples where each row
#                   corresponds to a single d-dimensional example
#
#   examples_Y --  a n-by-1 vector of example class labels
#
#   query --    a 1-by-d vector representing a single example
#
#   k --        the number of neighbors to return
#
# Output:
#   predicted_label --   either 0 or 1 corresponding to the predicted
#                        class of the query based on the neighbors
######################################################################

def knn_classify_point(examples_X, examples_y, query, k):

    norm_idx_of_nearest = get_nearest_neighbors_with_norm(examples_X, query, k)
    
    norm_sum = 0
    avg_norm = 0
    for index, norm in norm_idx_of_nearest:
        norm_sum += norm
    
    avg_norm = norm_sum / k

    weighted_vote_sum = [0.00001, 0.00001] #make sure no divide by 0
    
    for index, norm in norm_idx_of_nearest:
        weighted_vote_sum[int(examples_y[index][0])] += NORMAL_CONSTANT * math.pow(math.e, -math.pow((norm-avg_norm), 2) / (2 * SIGMA)) #sigmoid function result is tallied as vote
        
    normalized_vote = [weighted_vote_sum[0] / (weighted_vote_sum[0] + weighted_vote_sum[1]), weighted_vote_sum[1] / (weighted_vote_sum[0] + weighted_vote_sum[1])] #normalize between 0 and 1

    if normalized_vote[0] >= normalized_vote[1]:    #chose most likely
        predicted_label = 0
    else:
        predicted_label = 1
    
    return predicted_label




######################################################################
# Q8 cross_validation 
######################################################################
# Runs K-fold cross validation on our training data.
#
# Input: 
#   train_X --  a n-by-d matrix of examples where each row
#                   corresponds to a single d-dimensional example
#
#   train_Y --  a n-by-1 vector of example class labels
#
# Output:
#   avg_val_acc --      the average validation accuracy across the folds
#   var_val_acc --      the variance of validation accuracy across the folds
######################################################################

def cross_validation(train_X, train_y, num_folds=4, k=1):

    folded_X = np.array_split(train_X, num_folds)   #split data into n chunks
    folded_y = np.array_split(train_y, num_folds)

    
    accuracies = []

    for i in range(num_folds):
        folded_train_X = np.vstack(folded_X[:i] + folded_X[i + 1:]) #combine first part and last part together
        folded_train_y = np.vstack(folded_y[:i] + folded_y[i + 1:])

        correct = 0

        for index, query in enumerate(folded_X[i]):
            predicted_label = knn_classify_point(folded_train_X, folded_train_y, query, k)

            if predicted_label == int(folded_y[i][index][0]):
                correct += 1

        accuracies.append(correct / len(folded_y[i]))
    
    avg_val_acc = np.mean(accuracies)
    varr_val_acc = np.var(accuracies)



    return avg_val_acc, varr_val_acc



##################################################################
# Instructor Provided Code, Don't need to modify but should read
##################################################################


######################################################################
# compute_accuracy 
######################################################################
# Runs a kNN classifier on the query point
#
# Input: 
#   true_y --  a n-by-1 vector where each value corresponds to 
#              the true label of an example
#
#   predicted_y --  a n-by-1 vector where each value corresponds
#                to the predicted label of an example
#
# Output:
#   predicted_label --   the fraction of predicted labels that match 
#                        the true labels
######################################################################

def compute_accuracy(true_y, predicted_y):
    accuracy = np.mean(true_y == predicted_y)
    return accuracy

######################################################################
# Runs a kNN classifier on every query in a matrix of queries
#
# Input: 
#   examples_X --  a n-by-d matrix of examples where each row
#                   corresponds to a single d-dimensional example
#
#   examples_Y --  a n-by-1 vector of example class labels
#
#   queries_X --    a m-by-d matrix representing a set of queries 
#
#   k --        the number of neighbors to return
#
# Output:
#   predicted_y --   a m-by-1 vector of predicted class labels
######################################################################

def predict(examples_X, examples_y, queries_X, k): 
    # For each query, run a knn classifier
    predicted_y = [knn_classify_point(examples_X, examples_y, query, k) for query in queries_X]

    return np.array(predicted_y,dtype=int)[:,np.newaxis]

# Load data
def load_data():
    traindata = np.genfromtxt('train.csv', delimiter=',')[1:, 1:]
    train_X = traindata[:, :-1]
    train_y = traindata[:, -1]
    train_y = train_y[:,np.newaxis]
    
    print(np.sum(train_y[:,-1])/len(train_y))   #% income > 50k
    print(traindata.shape)   #dimension of table
    test_X = np.genfromtxt('test_pub.csv', delimiter=',')[1:, 1:]

    return train_X, train_y, test_X


    
if __name__ == "__main__":
    main()