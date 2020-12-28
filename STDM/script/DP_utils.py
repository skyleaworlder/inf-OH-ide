class Data_Process_1:
    def __init__(self):
        self.delta_lst = []
        self.mean_lst = []
        self.sigma_lst = []
        self.pfa_lst = []

    def process(self, records):
        for record in records:
            row = record.split(' ')
            self.delta_lst.append(float(row[0]))
            self.mean_lst.append(float(row[1]))
            self.sigma_lst.append(float(row[2]))
            self.pfa_lst.append(float(row[3]))

class Data_Process_2:
    def __init__(self):
        self.delta_lst = []
        self.q_level_lst = []
        self.pfa_lst = []

    def process(self, records):
        for record in records:
            row = record.split(' ')
            self.delta_lst.append(float(row[0]))
            self.q_level_lst.append(float(row[1]))
            self.pfa_lst.append(float(row[2]))