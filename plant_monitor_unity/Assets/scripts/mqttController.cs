using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class mqttController : MonoBehaviour
{
    public string nameController = "Controller 1";
    public mqttReceiver _eventSender;

    // Start is called before the first frame update
    void Start()
    {
        _eventSender.OnMessageArrived += OnMessageArrivedHandler;
    }

    private void OnMessageArrivedHandler(string newMsg)
    {
        this.GetComponent<TextMeshProUGUI>().text=newMsg;
        Debug.Log("Event Fired. The message, from Object " +nameController+" is = " + newMsg);

    }
}
