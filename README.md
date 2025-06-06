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

### Server Connection System (5/2)
* add Title Level (add BP_GameModeBase_Title, DXTitlePlayerController, UW_TitleLayout) for Connection Scene
* implement game flow system (update DXGameModeBase, DXGameStateBase code) 
  * wait for player
  * count down for play
  * show current state by notification text(WBP_NotificationText)
 <br/>

https://github.com/user-attachments/assets/7f762e4f-ef79-4d97-850b-f97a491602bd

### Combo Attack System (6/2)
* add Player Combo Attack System (use animation montage)
  * use section system
  * use Animation Notify State
<br/>

![스크린샷 2025-06-02 020059](https://github.com/user-attachments/assets/40fe0e61-7633-4dbf-bb90-e0a31993f8d8)
<br/>

https://github.com/user-attachments/assets/df97830a-fb71-4b45-83cb-f1ec00e1dce3



 
