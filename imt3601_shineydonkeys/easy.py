#python level editor

import numpy as np
import pygame
from pygame.locals import *


f = open('level02.txt', 'r')
print f

vec=[]

for line in f:
	vec.append(int(line))

print vec

print len(vec)
print 44*87

(width, height) = (1000, 600)
screen = pygame.display.set_mode((width, height))
pygame.display.flip()


#x = np.array(vec, np.int32)
#np.reshape(x,(44,87))


if 1:
# Fill background
	background = pygame.Surface(screen.get_size())
	background = background.convert()
	background.fill((50, 50, 50))

	colors=((0,0,255),(66,66,66),(122,122,122),(99,199,199),(0,230,0),(0,180,0),(10,10,10))



	# Blit everything to the screen
	screen.blit(background, (0, 0))
	pygame.display.flip()
	print pygame.mouse.get_pos()

while 1:


		for event in pygame.event.get():
			if event.type == QUIT:
				pygame.quit()

				f = open('level02.txt', 'w')
				print f
				vecindex=0
				for tile in vec:
					if (vecindex<45):
						tile=0
					f.write(str(tile)+" \n")
					vecindex=vecindex+1


				exit()

		vecindex=1
		for x in range(86):
			for y in range(44):
				currentcolor=colors[vec[vecindex]%7]
				pygame.draw.rect(background,currentcolor , (y*11,x*6,10,5))
				getx=pygame.mouse.get_pos()[0]
				gety=pygame.mouse.get_pos()[1]
				if getx>y*11 and getx<y*11+10 and gety>x*6 and gety<x*6+5:
					pygame.draw.rect(background,(255,255,255) , (y*11,x*6,10,5))
				vecindex=vecindex+1

		
		#pygame.draw.rect(background,currentcolor , (pygame.mouse.get_pos()[0],pygame.mouse.get_pos()[1],10,5))

		screen.blit(background, (0, 0))

		pygame.display.flip()