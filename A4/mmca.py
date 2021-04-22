def mmca(a, b):
    infected_old = infected_original[:]
    for _ in range(10000):
        infected_new = []
        for i, state in enumerate(infected_old):
            if state:
                new_state = 1-a
            else:
                q = 1
                for neighbor in neighbors[i]:
                    q *= 1 - b*infected_old[neighbor]
                new_state = 1-q
            infected_new.append(new_state)
        first = infected_new[0]
        if all(first == x for x in infected_new):
            print(a, b, first)
            break
        infected_old = infected_new
    else:
        print(a, b, "Error")