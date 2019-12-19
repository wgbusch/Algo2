import matplotlib.pyplot as plt
import numpy as np
import sys
import seaborn as sns
import os
from subprocess import Popen, PIPE, STDOUT
import scipy.ndimage as ndimage
from collections import defaultdict


def rgb2gray(rgb):
    # ITU-R 601-2 luma transform
    return np.dot(rgb[..., :3], [0.2989, 0.5870, 0.1140])


def read_image(image_file_name):
    img_matrix = plt.imread(image_file_name)
    return img_matrix


def preprocess_image(img_matrix):
    img_matrix = rgb2gray(img_matrix)
    img_matrix = ndimage.gaussian_filter(img_matrix, sigma=2.5)
    return img_matrix


def coord_to_node(x, y, width):
    return y * width + x


def image_to_graph(image_matrix):
    height, width = image_matrix.shape

    nodes = []
    edges = []
    node = 0

    for y in range(height):
        for x in range(width):
            nodes.append(node)
            node += 1

            # Down
            if y + 1 < height:
                edges.append((coord_to_node(x, y, width), coord_to_node(x, y + 1, width), int(round(abs(image_matrix[y][x] - image_matrix[y + 1][x])))))

            # Right
            if x + 1 < width:
                edges.append((coord_to_node(x, y, width), coord_to_node(x + 1, y, width), int(round(abs(image_matrix[y][x] - image_matrix[y][x + 1])))))

    return nodes, edges


def write_graph(nodes, edges):
    s = ''
    s = s + "%s\t%s\n" % (len(nodes), len(edges))
    for edge in edges:
        s = s + "%s\t%s\t%s\n" % edge
    return s


def run_algorithm(graph, k):
    algorithm_input = str(k) + '\t' + graph
    dir_path = os.path.dirname(os.path.realpath(__file__))

    p = Popen([dir_path + '/../c++/segment_image'], stdout=PIPE, stdin=PIPE, stderr=STDOUT)
    stdout = p.communicate(input=bytes(algorithm_input, "ascii"))[0]
    return stdout.decode()


def segment_image(image_file, k):
    original_image_matrix = read_image(image_file)

    image_matrix = preprocess_image(original_image_matrix)
    height, width = image_matrix.shape

    nodes, edges = image_to_graph(image_matrix)
    graph = write_graph(nodes, edges)
    segment_per_pixel = run_algorithm(graph, k).split(',')
    segment_per_pixel = segment_per_pixel[:-1]

    segmented_image = np.array([int(s) for s in segment_per_pixel]).reshape(height, width)

    colors = defaultdict(list)
    # Get averages
    for y in range(height):
        for x in range(width):
            colors[segmented_image[y][x]].append(original_image_matrix[y][x])

    final_colors = {}
    for segment, colors in colors.items():
        final_colors[segment] = (
            int(round(sum([c[0] for c in colors]) / len(colors))),
            int(round(sum([c[1] for c in colors]) / len(colors))),
            int(round(sum([c[2] for c in colors]) / len(colors))),
        )

    segment_per_pixel = np.array([final_colors[int(s)] for s in segment_per_pixel]).reshape(height, width, 3)

    return segment_per_pixel
