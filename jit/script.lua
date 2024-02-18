-- Sleep
print("Sleep")
Sleep(500)
print("Sleep End")

-- OutputLogMessage
OutputLogMessage("OutputLogMessage")
OutputLogMessage("OutputLogMessage", 1)
OutputLogMessage()

-- OutputDebugMessage
OutputDebugMessage("OutputDebugMessage")
OutputDebugMessage("OutputDebugMessage", 1)
OutputDebugMessage()

-- GetRunningTime
OutputLogMessage(GetRunningTime())

-- GetDate
OutputLogMessage(GetDate())
OutputLogMessage(GetDate("%Y-%m-%d %H:%M:%S"))
OutputLogMessage(GetDate("%Y-%m-%d %H:%M:%S", 1))

-- EnablePrimaryMouseButtonEvents
EnablePrimaryMouseButtonEvents(true)

-- OnEvent
function OnEvent(event, arg, family)
    OutputLogMessage("OnEvent: " .. event .. " " .. arg .. " " ..family)
end
