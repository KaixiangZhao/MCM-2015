# MCM-2015: Integrated Management Model for Human Capital

------------------

This repository provides an implementation for Mathematical Contest in Modeling 2015. Detail information is in 2015Summary.pdf.

## Abstract
The organizational churn is a term to describe the turbulence caused by some employees leaving for other jobs. Since the HR manager wants to automate and analyze the human capital using network science, We employ a Human Capital Model and it's improved model to simulate this phenomenon and analyze its influence.

## Brief Explaination of Implementation
- **Task 1:** This project constructs the directed weighted employee network based on the 370th-order relationship matrix. Proposed an Employee Relation Model based on the SNA (social network analysis) technique, it calculates the "distance" between employees considering two main aspects in human relation. Meanwhile, for each node (employee) in the network, it has many attributes which are used to evaluate the employee correspond to the node. This project calculates the grade of each employee using these evaluations based on AHP (analytic hierarchy process) technique. With this model, it predicts the promotion and churn rate for each employee in the company.

- **Task 2:** This project establishes an Organizational Churn Model to simulate the effect caused by the leaving of some employees. What's more, it realizes a new method to calculate productivity considering 5 factors which are churn rate, team cohesiveness, ability, time duration and salary.

- **Task 3 & 4:** This project calculates the budget for the next two years and analyze the influence of different churn rate. The cost of recruitment increase and the cost of training and salary decrease with the churn rate going higher.

- **Task 5:** This project realizes the situation which doesn't have recruitment, and get the result that the amount of emloyees decreases rapidly.

- **Task 6:** After discussing the potential use of team science and multi-layered network, it is applied to the model.

## Additional Functions
- This project proposes a further improved model which can analyze the influence of reassignment and give incentive mechanism to HR managers.
- This project conducts a sensitivity analysis to study the robustness of our algorithm, and the results show a good stability. 
