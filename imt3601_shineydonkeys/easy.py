#python level editor

import numpy as np
import pygame
from pygame.locals import *


f = open('level02.txt', 'r')
print f

vec=[]
brushsize=2
selectedcolor=(0,0,0)
selectedcolorindex=1
for line in f:
	vec.append(int(line))

#print vec

#print len(vec)
#print 44*87

(width, height) = (1000, 600)
screen = pygame.display.set_mode((width, height))
pygame.display.flip()




framecount=0

if 1:
	framecount+=1
	if framecount%10==0:
		print framecount,pygame.mouse.get_pressed()

# Fill background
	background = pygame.Surface(screen.get_size())
	background = background.convert()
	background.fill((50, 50, 50))

	colors=((0,0,255),(66,66,66),(122,122,122),(199,199,199),(0,230,0),(0,180,0),(10,10,10))



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
		getx=pygame.mouse.get_pos()[0]
		gety=pygame.mouse.get_pos()[1]
		vecindex=1
		for x in range(86):
			for y in range(44):
				currentcolor=colors[vec[vecindex]%7]
				pygame.draw.rect(background,currentcolor , (y*11,x*6,10,5))
				
				if getx>y*11 and getx<y*11+10 and gety>x*6 and gety<x*6+5:
					pygame.draw.rect(background,(255,255,255) , (y*11,x*6,10,5))
					if not pygame.mouse.get_pressed()==(0,0,0):
						#pygame.draw.rect(background,selectedcolor , (y*11,x*6,10,5))
						for xx in range(brushsize):
							for yy in range(brushsize):
						  		vec[(vecindex+xx+yy*44)%(44*87)]=selectedcolorindex-1
						
						

				vecindex=vecindex+1
		vecindex=1
		for x in colors:
			
			pygame.draw.rect(background,x , (500+vecindex*50,50,50,50))
			if 550+vecindex*50>getx>500+vecindex*50 and 100>gety>50 and not pygame.mouse.get_pressed()==(0,0,0):
				selectedcolor=x
				selectedcolorindex=vecindex
				pygame.draw.rect(background,x , (700,300,100,100))
				pass


			if vecindex<5:
				pygame.draw.rect(background,x , (500+vecindex*50+50-vecindex*5,150+50-vecindex*5,vecindex*10,vecindex*10))
				if getx>500+vecindex*50+50-vecindex*5 and getx<500+vecindex*50+50+vecindex*5 and gety>150+50-vecindex*5 and gety<150+50+vecindex*5:
					selectedcolor=x

					pygame.draw.rect(background,x , (550,300,100,100))
					brushsize=vecindex
					print brushsize
					pass

			vecindex+=1


		
		#pygame.draw.rect(background,currentcolor , (pygame.mouse.get_pos()[0],pygame.mouse.get_pos()[1],10,5))

		screen.blit(background, (0, 0))

		pygame.display.flip()