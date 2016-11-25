# Finds the n_epochs epochs with the highest average reward in the test phase


from sys import argv

n_epochs = 6
best_rewards = [(0, -10000000000)]*n_epochs # initial value
e_idx = 0

with open(argv[1], "r") as f:
    rows = f.readlines()
    for r in rows:
        values = r.split(",")
        if values[1] != "test":
            continue
        e_idx += 1
        reward = float(values[4])

        if reward > best_rewards[-1][1]:
            best_rewards[-1] = (e_idx,reward)
            best_rewards.sort(reverse=True, key=lambda x: x[1])
    f.close()

print(best_rewards)
