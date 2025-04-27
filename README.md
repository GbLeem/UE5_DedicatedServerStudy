# UE5_DedicatedServerStudy

### DXLandMine Actor - Using Property Replication (4/10)
* Apply NetCullDistance
* Explosion effect with Replication
* Change Material on Collision with Character (Using `OnRep` function) </br>

https://github.com/user-attachments/assets/5eec3b83-90fd-4534-8372-4233b7649b4b

### DXPlayerCharacter - Movement Replication (4/10)
* using movement replication
<br/>

https://github.com/user-attachments/assets/889c5024-b579-4536-9f21-1aacbad226c2

### Attack System Synchronization (4/11)
* prevent server overload (server logic to client)
* improve damage logic (for better user experience)
* improve anim montage play code (use Client RPC instead of NetMulticast) <br/>

https://github.com/user-attachments/assets/6d2e1f5a-7239-417c-8e0d-1bb8247c92e3

### Status Component & Widget Component (4/12)
* add actor component for managing health
* add widget component for update health widgets
* use Mulicast Delegates
  * when the delegate variable changes
  * call the Broadcast() function
  * finally call the bind function
<br/>

https://github.com/user-attachments/assets/aa0d2ded-be14-4d70-9354-72e1cbd2c567

### Condition Property Replication (4/14)
* add DXBuffBox for Condition Property Replication test
* using `DOREPLIFETIME_CONDITION`
* by using `COND_OwnerOnly`, max health value visible only to the owner
<br/>

![image](https://github.com/user-attachments/assets/7674c17e-ff47-492f-a413-afedf157615b)
