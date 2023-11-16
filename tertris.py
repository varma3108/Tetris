from itertools import combinations_with_replacement

def tetris_probability(width, height, lines_to_win):
    base = height + 1  # Base for encoding states
    
    # Memoization dictionary for known probabilities
    known_probabilities = {}
    
    # Helper function to calculate the probability of a given state
    def state_probability(state):
        # Sort the state to consider identical permutations as one
        state = tuple(sorted(state))
        
        # If the state is already known, return its probability
        if state in known_probabilities:
            return known_probabilities[state]
        
        # Check for win/loss conditions
        if max(state) >= height:  # Loss condition: any column exceeds the play area height
            known_probabilities[state] = 0
            return 0
        if all(x >= lines_to_win for x in state):  # Win condition: all columns have enough lines
            known_probabilities[state] = 1
            return 1
        
        # Calculate probabilities of next states
        next_probabilities = []
        for i in range(width):
            # Increase the height of the ith column by 1
            next_state = list(state)
            next_state[i] += 1
            # Recursively calculate the probability of the next state
            next_prob = state_probability(next_state)
            next_probabilities.append(next_prob)
        
        # The probability of the current state is the average of the next states' probabilities
        prob = sum(next_probabilities) / width
        known_probabilities[state] = prob
        return prob
    
    # Start the calculation from the initial state (all columns empty)
    initial_state = (0,) * width
    return state_probability(initial_state)

# Example usage:
# Play area: 4x2, lines to win: 2
print(tetris_probability(2, 4, 2))
