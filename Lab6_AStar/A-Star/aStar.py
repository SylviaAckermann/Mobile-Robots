#!/usr/bin/env python3
from eye import *
import math
from queue import PriorityQueue


# global variables
FILENAME = "branch.txt"


class Node:
    def __init__(self, id: int, x: int, y: int):
        self.id = id
        self.x = x
        self.y = y
        self.neighbours = []

    def addNeighbour(self, neighbour):
        self.neighbours.append(neighbour)


class Edge:
    def __init__(self, neighbour: Node, distance: int):
        self.neighbour = neighbour
        self.distance = distance


def getRelativeGoalLocation(x, y):  # -> (int, int):
    desiredXPosition = x
    desiredYPosition = y

    currentXPosition, currentYPosition, currentAngle = VWGetPosition()

    # calculate movement required to get to new point
    sPhi = math.atan2(desiredYPosition-currentYPosition,
                      desiredXPosition-currentXPosition)*180.0/math.pi
    rot = sPhi - currentAngle
    distance = math.sqrt(pow(desiredXPosition-currentXPosition,
                         2) + pow(desiredYPosition-currentYPosition, 2))
    return int(distance), int(rot)


def driveToPoint(x, y):
    distance, rotation = getRelativeGoalLocation(x, y)
    VWTurn(rotation, 25)
    VWWait()
    VWStraight(distance, 50)
    VWWait()


def printAdjacencyMatrix():
    print('\nAdjacent Matrix: ')
    for node in nodes:
        distances = [-1]*len(nodes)
        for edge in node.neighbours:
            distances[edge.neighbour.id-1] = edge.distance

        distances[node.id-1] = 0
        output = ''
        for distance in distances:
            output += '%.2f\t' % distance
        print(output)
    return


def heuristic(node: Node):
    # get euclidian distance
    endNode = nodes[len(nodes) - 1]
    return math.sqrt((node.x-endNode.x)**2 + (node.y-endNode.y)**2)


def readFile():
    global nodes
    with open(FILENAME, "r") as f:
        lines = f.readlines()
        nodes = [None] * len(lines)
        id = 1

        for line in lines:
            entries = [int(i) for i in line.strip().split()]
            node = nodes[id-1]
            if node is None:
                node = Node(id, entries[0], entries[1])
                nodes[id-1] = node

            neighbours = entries[2:]
            LCDCircle(2*int(128*node.x/2000), 256-2 *
                      int(128*node.y/2000), 7, WHITE, 1)

            for neighbour in neighbours:
                neighbourNode = nodes[neighbour-1]
                if neighbourNode is None:
                    neighbourEntries = [
                        int(x) for x in lines[neighbour - 1].strip().split()]
                    neighbourNode = Node(
                        neighbour, neighbourEntries[0], neighbourEntries[1])
                    nodes[neighbour-1] = neighbourNode

                distance = math.sqrt(
                    (node.x-neighbourNode.x)**2 + (node.y-neighbourNode.y)**2)
                node.addNeighbour(Edge(neighbourNode, distance))
                LCDLine(2*int(128*node.x/2000), 256-2*int(128*node.y/2000), 2 *
                        int(128*neighbourNode.x/2000), 256-2*int(128*neighbourNode.y/2000), WHITE)

            id += 1
    for n in nodes:
        print("Node ", n.id, " (", n.x, ",", n.y, ")")


def getPathAStar():
    inf = 99999999
    startNode = nodes[0]
    endNode = nodes[len(nodes)-1]
    openSet = []
    closedSet = []
    parent = {}
    gScores = {}
    fScores = {}

    # initialise fScores, gScores and openSet
    currentNode = startNode
    for n in currentNode.neighbours:
        openSet.append(n.neighbour)
        gScores.update({n.neighbour.id: n.distance})
        fScores.update({n.neighbour.id: n.distance+heuristic(n.neighbour)})
        parent.update({n.neighbour: currentNode})
    closedSet.append(startNode)

    while len(openSet) != 0:

        # get node in openset with lowest f score
        sortedScores = sorted(fScores.items(), key=lambda x: x[1])
        currentNodeID = sortedScores[0][0]

        for node in nodes:
            if (node.id == currentNodeID):
                current = node

        if current.id == endNode.id:  # at the goal
            # Retrieve path from parents and return
            currentNode = endNode
            path = []
            previousNode = endNode
            # totalDistance = 0

            # Update total distance
            totalDistance = gScores.get(currentNode.id)  # += ?

            while currentNode != startNode:
                # add to the path
                path.append(currentNode)
                # Update the current and previous node
                previousNode = parent.get(currentNode)
                currentNode = previousNode

            path.append(currentNode)  # The last node
            path.reverse()  # Put it in the right order

            print("Path distance: ", totalDistance)
            print("shortest path: ", [node.id for node in path])
            return path

        openSet.remove(current)
        closedSet.append(current)

        neighbourGScore = inf
        for edge in current.neighbours:
            neighbour = edge.neighbour

            if neighbour in closedSet:
                continue

            # calculate from gScore and edge distance
            currentGScore = gScores.get(currentNodeID)
            tentativeGScore = currentGScore + edge.distance
# '''
            # tentative path is better than recorded path
            if tentativeGScore < neighbourGScore:
                # record parent
                parent.update({neighbour: current})
                # record gScore and fScore using heuristic
                neighbourGScore = tentativeGScore
                gScores.update({neighbour.id: tentativeGScore})
                fScores.update(
                    {neighbour.id: tentativeGScore+heuristic(neighbour)})

                if neighbour not in openSet:
                    openSet.append(neighbour)
        fScores.pop(current.id)
        gScores.pop(current.id)

    print("No path found")
    return []
# '''


# ************************** MAIN ******************************************
if __name__ == "__main__":
    # SIMSetRobot(0, 300, 300, 100, -90)
    VWSetPosition(100, 100, 90)

    LCDMenu("Start", "A*", "Drive", "End")

    KEYWait(KEY1)
    LCDPrintf("Load File\n")
    print("\n\n Loading File \n\n")
    readFile()
    printAdjacencyMatrix()

    KEYWait(KEY2)
    LCDPrintf("Find Shortest Path\n")
    print("\n\n Find Shortest Path \n\n")
    path = getPathAStar()

    KEYWait(KEY3)
    LCDPrintf("DRIVE\n")
    for node in path:
        LCDCircle(2*int(128*node.x/2000), 256-2 *
                  int(128*node.y/2000), 7, BLUE, 1)
    for node in path:

        driveToPoint(node.x, node.y)
        LCDCircle(2*int(128*node.x/2000), 256-2 *
                  int(128*node.y/2000), 7, RED, 1)

    KEYWait(KEY4)
