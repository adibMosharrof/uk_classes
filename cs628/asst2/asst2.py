
import numpy as np
from sklearn.cluster import KMeans
import matplotlib.pyplot as plt
from pyclustering.cluster.cure import cure
from pyclustering.cluster import cluster_visualizer

class Clustering():

	def generate_points(self, n=1000):
		limit = 10000
		return limit* np.random.randn(1000,2)

	def kmeans_cluster(self, k=2):
		kmeans = KMeans(n_clusters=k, random_state=0).fit(self.points)
		return kmeans.labels_

	def visualize(self, labels, k=2):
		# colors = np.random.rand(len(labels))
		for i in range(1, k+1):
			indices = labels == i
			filtered_points = self.points[indices]
			x = [item[0] for item in filtered_points]
			y = [item[1] for item in filtered_points]
			plt.scatter(x,y)

	def cure_clustering(self, k=2):
		cure_instance = cure(self.points, k)
		cure_instance.process()
		clusters = cure_instance.get_clusters()
		visualizer = cluster_visualizer()
		visualizer.append_clusters(clusters, self.points)
		# visualizer.show()
		visualizer.save(f'cure_k_{k}.png')

	def run(self):
		self.points = self.generate_points()
		np.savetxt('points.txt', self.points, delimiter=',')
		for i in range(2,5):
			#kmeans clustering
			labels = self.kmeans_cluster(k=i)
			self.visualize(labels)
			plt.legend()
			plt.title(f'K={i}')
			plt.savefig(f'Kmeans_k_{i}.png')	
			#cure clustering
			self.cure_clustering(k=i)
		# labels = self.kmeans_cluster()
		# self.visualize(labels)

if __name__ == "__main__":
	c = Clustering()
	c.run()
	# points = generate_points()
	
	