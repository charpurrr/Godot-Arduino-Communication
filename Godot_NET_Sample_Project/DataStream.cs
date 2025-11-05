using Godot;
using System;
using System.Diagnostics;
using System.Text.RegularExpressions;


public partial class DataStream : Node
{
	[Export]
	private string IP = "10.120.3.173";
	[Export]
	private int Port = 80;

	[ExportCategory("References")]
	[Export]
	public Button Button;
	[Export]
	public Label StatusLabel;
	[Export]
	public Label DataLabel;

	private StreamPeerTcp Client = new StreamPeerTcp();


	public override void _Ready()
	{
		AttemptConnect();
	}


	public override void _Process(double delta)
	{
		if (Client == null)
		{
			GD.PrintErr("No client connected.");
			return;
		}

		Client.Poll();

		switch (Client.GetStatus())
		{
			case StreamPeerTcp.Status.Connected:
				StatusLabel.Text = "Connected";
				HandleOutgoing();
				HandleIncoming();
				break;
			case StreamPeerTcp.Status.Connecting:
				StatusLabel.Text = "Connecting...";
				break;
			case StreamPeerTcp.Status.None:
			case StreamPeerTcp.Status.Error:
				StatusLabel.Text = "Disconnected. Retrying...";
				AttemptConnect();
				break;
		}
	}


	private void HandleOutgoing()
	{
		string Outgoing = Button.ButtonPressed + "\n";
		Client.PutData(Outgoing.ToUtf8Buffer());
	}


	private void HandleIncoming()
	{
		int Incoming = Client.GetAvailableBytes();

		if (Incoming > 0)
		{
			string Msg = Client.GetUtf8String(Incoming);
			string MsgClean = Msg.Trim();

			if (MsgClean != string.Empty)
			{
				GD.Print(MsgClean);
				DataLabel.Text = MsgClean;
			}
		}
	}


	private void AttemptConnect()
	{
		Client = new StreamPeerTcp();
		Error ERR = Client.ConnectToHost(IP, Port);

		if (ERR == Error.Ok)
		{
			GD.Print("Attempting connection to host.");
		}
		else
		{
			GD.PrintErr("Failed to connect. Error: " + ERR);
		}
	}
}
