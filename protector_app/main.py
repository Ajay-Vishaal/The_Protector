'''
*****************************************************************************************
*
*        		===============================================
*           		The Protector Android App
*        		===============================================
*
*  This script is the source code of the Protector App which uses Kivy an opensource framework for 
developing mobile apps.
*
*
# Author:   Ajay Vishaal T
# Filename: main.py
*****************************************************************************************
'''



# Import necessary modules
import kivy  
from kivy.app import App
from kivy.uix.button import Button
from kivy.uix.textinput import TextInput
from kivy.uix.label import Label
from kivy.uix.switch import Switch 
from kivy.clock import Clock
from kivy.uix.relativelayout import RelativeLayout
from kivy.uix.gridlayout import GridLayout 
from kivy.config import Config 
from kivy.uix.pagelayout import PageLayout
import urllib.request
from bs4 import BeautifulSoup
import time

#configure app window
Config.set('graphics', 'resizable', True)

#IP address of the Arduino MKR wifi 1010
root_url = "http://<Your MKR's IP address>"

#function to acess the device's IP address
def sendRequest(url):
	n = urllib.request.urlopen(url)

#main class of the app	
class ProtectorApp(App):
 def build(self):
        
        #Creates a grid layout to hold the app interfaces like buttons,switches,labels etc,.                   
        rl = GridLayout(rows = 14)
         
        #creates a main label for app title
        l = Label(text = "The Protector Dashboard",
                      font_size = 30, 
                      size_hint =(.3, .3),  
                      pos_hint ={'right_x':.5, 'center_y':.5}, 
                      )
        
        #creates a label for displaying limit              
        n = Label(text = "Customer Limit", 
                      size_hint =(.2, .2),  
                      pos_hint ={'right_x':.5, 'center_y':.5}, 
                      )
                                   
        #creates a dyanamic label for updating limit
        self.limit = Label(text = '', size_hint =(.1, .1))
  
        #creates a label for switch1 title
        l2 = Label(text = "Socket", 
                      size_hint =(.2, .2),  
                      pos_hint ={'right_x':.5, 'center_y':.5}, 
                      )     
                    
        #creates a label for switch2 title              
        l3 = Label(text = "Light", 
                      size_hint =(.2, .2),  
                      pos_hint ={'right_x':.5, 'center_y':.5}, 
                      )
                    
        #creates a label for switch3 title            
        l4 = Label(text = "Fan", 
                      size_hint =(.2, .2),  
                      pos_hint ={'right_x':.5, 'center_y':.5}, 
                      )
                        
        #creates a label to display persons count                
        l5 = Label(text = "Number of Persons", 
                      size_hint =(.2, .2),  
                      pos_hint ={'right_x':.5, 'center_y':.5}, 
                      )
                                                 
        #creates a dynamic label to show the person count                                                         
        self.output_label = Label(text = '', size_hint =(.1, .1))
        
        #creates a button instance               
        b1 = Button(size_hint = (.2, .1),  
                    text ="+")  
        #binds a function to the instance            
        b1.bind(state=self.on_up)           
        
        #creates a button instance
        b2 = Button(size_hint = (.2, .1),  
                    text ="-")  
        #binds a function to the instance            
        b2.bind(state=self.on_down)
        
        #creates switch instance
        self.s = Switch(active = False, size_hint = (.1,.1))
        self.s2 = Switch(active = False, size_hint = (.1,.1))
        self.s3 = Switch(active = False, size_hint = (.1,.1))
       
        #adds all the widgets to the layout
        rl.add_widget(l)
        rl.add_widget(n)
        rl.add_widget(b1)
        rl.add_widget(self.limit)
        rl.add_widget(b2)
        rl.add_widget(l5)
        rl.add_widget(self.output_label)
        rl.add_widget(l2)
        rl.add_widget(self.s)
        rl.add_widget(l3)
        rl.add_widget(self.s2)
        rl.add_widget(l4)
        rl.add_widget(self.s3)
                        
        #binds functions to the switch instances                
        self.s.bind(active_norm_pos = self.on_press_sw1)
        self.s2.bind(active_norm_pos = self.on_press_sw2)
        self.s3.bind(active_norm_pos = self.on_press_sw3)
        #create a scheduled clock to update label parameters       
        Clock.schedule_interval(self.person, 1)
        return rl

 #function called during switch actions
 def on_press_sw1(self,instance,value): 
  if (value == True):
   sendRequest(root_url+"/H")
   
  if (value == False):
   sendRequest(root_url+"/L")
   
 #function called during switch actions  
 def on_press_sw2(self,instance,value): 
  if (value == True):
   sendRequest(root_url+"/O")
   
  if (value == False):
   sendRequest(root_url+"/C")
   
 #function called during switch actions  
 def on_press_sw3(self,instance,value): 
  if (value == True):
   sendRequest(root_url+"/F")
     
  if (value == False):
   sendRequest(root_url+"/B")
   
 #function called during button action  
 def on_up(self,instance,value):
  if (value == "down"):
   sendRequest(root_url+"/U")  
    
 #function called during button action   
 def on_down(self,instance,value):
  if (value == "down"): 
   sendRequest(root_url+"/D")
    
 #function accessed by clock at an interval of 1 sec   
 def person(self,instance):
   #access the devices IP address and reads the data
   html_doc = urllib.request.urlopen(root_url)
   soup = BeautifulSoup(html_doc, 'html.parser')
   #extracts data from the particular tags
   x = soup.find('t1') 
   l = soup.find('t2')
   cc1 = soup.find('c1')
   cc2 = soup.find('c2')
   cc3 = soup.find('c3') 
   #removes spaces from the retrived data
   c1 = cc1.string.strip()
   c2 = cc2.string.strip()
   c3 = cc3.string.strip()
   txt = str(x.string.lstrip())
   n = str(l.string.lstrip())
   #update the data to labels
   self.output_label.text = txt
   self.limit.text = n
   #checks the data with reference value
   if (c1 == "1"):
    #if true changes state of switch
    self.s.active = True
   #checks the data with reference value
   if (c1 == "0"):
    #if true changes state of switch
    self.s.active = False
   
   #checks the data with reference value
   if (c2 == "1"):
    #if true changes state of switch
    self.s2.active = True      
   #checks the data with reference value
   if (c2 == "0"):
    #if true changes state of switch
    self.s2.active = False     
   
   #checks the data with reference value
   if (c3 == "1"):    
    #if true changes state of switch     
    self.s3.active = True
   #checks the data with reference value
   if (c3 == "0"):
    #if true changes state of switch
    self.s3.active = False
    
#function to run the app   
if __name__ == "__main__":
    app = ProtectorApp()
    app.run()
