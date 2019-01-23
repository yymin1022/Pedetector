<?php
    function send_notification ($message)
    {
        include_once 'config.php';
            //URL ���� �Ķ���͸� topic���� ����
            $url = 'https://fcm.googleapis.com/fcm/send';
            $topic = '/topics/'.$_GET['devcode'];
            $priority_set = 'high';
            $fielde = array('to' => $topic, 'data' => $message, 'priority' => $priority_set);
            $headers = array('Authorization:key ='.GOOGLE_API_KEY, 'Content-Type: application/json');
        //HTTP Request �ۼ�
        $ch = curl_init();
        curl_setopt($ch, CURLOPT_URL, $url);
        curl_setopt($ch, CURLOPT_POST, true);
        curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
        curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
        curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
        curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($fields));
        $result = curl_exec($ch);
        if($result === FALSE){
            die('Curl Failed: '.curl_error($ch));
        }
        curl_close($ch);
        return $result;
    }
    //�޽����� ��ȯ �� ���� �õ�
    $myMessage = $_POST['message'];
    if($myMessage == ""){
        $myMessage = "�ֺ� ".$topic." ��ȣ� ������� �����ϴ�.";
    }
    $message = array("message" => $myMessage);
    $message_status = send_notification($message);
    //�޽��� ���� �� ��� ���
    echo $message_status;
?>